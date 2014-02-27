
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "yotta_socket_thread.h"
#include "../yotta_debug.h"
#include "../yotta_return.h"
#include "../core/yotta_logger.private.h"


enum
{
    YOTTA_SOCKET_THREAD_CONTINUE,
    YOTTA_SOCKET_THREAD_STOP_ON_EMPTY,
    YOTTA_SOCKET_THREAD_STOP_NOW
};

static
void *
yotta_socket_thread_main(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    fd_set fd_set_recv;
    fd_set fd_set_send;
    fd_set fd_set_except;
    struct timeval timer;

    timer.tv_sec = 0;
    timer.tv_usec = 20000;

    while (thread->quit_status != YOTTA_SOCKET_THREAD_STOP_NOW)
    {
        int32_t fd_max = 0;

        FD_ZERO(&fd_set_recv);
        FD_ZERO(&fd_set_send);
        FD_ZERO(&fd_set_except);

        pthread_mutex_lock(&thread->mutex);
        {
            yotta_socket_event_t * socket_cursor = thread->socket_head;

            if (socket_cursor == 0 && thread->quit_status == YOTTA_SOCKET_THREAD_STOP_ON_EMPTY)
            {
                pthread_mutex_unlock(&thread->mutex);
                break;
            }

            while (socket_cursor)
            {
                yotta_assert(socket_cursor->except_event != 0);

                int32_t fd = (int32_t)socket_cursor->socket.fd;

                fd_max = (fd > fd_max) ? fd : fd_max;

                FD_SET(fd, &fd_set_except);

                if (socket_cursor->recv_event)
                {
                    FD_SET(fd, &fd_set_recv);
                }

                if (socket_cursor->send_event)
                {
                    FD_SET(fd, &fd_set_send);
                }

                socket_cursor = socket_cursor->socket_next;
            }
        }
        pthread_mutex_unlock(&thread->mutex);

        int32_t event_counts = select(fd_max + 1, &fd_set_recv, &fd_set_send, &fd_set_except, &timer);

        if (event_counts == -1)
        {
            yotta_logger_error("yotta_socket_thread_main: select error");

            return 0;
        }

        pthread_mutex_lock(&thread->mutex);

        yotta_socket_event_t * socket_cursor = thread->socket_head;

        while (event_counts != 0 && socket_cursor != 0)
        {
            yotta_socket_event_t * socket_event = socket_cursor;

            /*
             * we move to the next socket event because the current one might be
             * destroyed in the exception callback
             */
            socket_cursor = socket_cursor->socket_next;

            int32_t fd = (int32_t)socket_event->socket.fd;

            pthread_mutex_unlock(&thread->mutex);

            if (FD_ISSET(fd, &fd_set_except))
            {
                socket_event->except_event(socket_event);
                event_counts -= 1;

                if (FD_ISSET(fd, &fd_set_recv))
                {
                    event_counts -= 1;
                }

                if (FD_ISSET(fd, &fd_set_send))
                {
                    event_counts -= 1;
                }

                pthread_mutex_lock(&thread->mutex);
                continue;
            }

            if (FD_ISSET(fd, &fd_set_recv))
            {
                socket_event->recv_event(socket_event);
                event_counts -= 1;
            }

            if (FD_ISSET(fd, &fd_set_send))
            {
                socket_event->send_event(socket_event);
                event_counts -= 1;
            }

            pthread_mutex_lock(&thread->mutex);
        }

        pthread_mutex_unlock(&thread->mutex);
    }

    return 0;
}

uint64_t
yotta_socket_thread_init(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    thread->socket_head = 0;
    thread->quit_status = YOTTA_SOCKET_THREAD_CONTINUE;

    if (pthread_mutex_init(&thread->mutex, 0) != 0)
    {
        return -1;
    }

    if (pthread_create(&thread->id, 0, (void *(*)(void *))yotta_socket_thread_main, thread) != 0)
    {
        pthread_mutex_destroy(&thread->mutex);
        return -1;
    }

    return 0;
}

uint64_t
yotta_socket_thread_listen(yotta_socket_thread_t * thread, yotta_socket_event_t * socket_event)
{
    yotta_assert(thread != 0);
    yotta_assert(socket_event != 0);
    yotta_assert(socket_event->socket_thread == 0);
    yotta_assert(socket_event->except_event != 0);
    yotta_assert(socket_event->release_event != 0);

    socket_event->socket_thread = thread;

    pthread_mutex_lock(&thread->mutex);
    {
        socket_event->socket_next = thread->socket_head;
        thread->socket_head = socket_event;
    }
    pthread_mutex_unlock(&thread->mutex);

    return 0;
}

uint64_t
yotta_socket_thread_unlisten(yotta_socket_thread_t * thread, yotta_socket_event_t * socket_event)
{
    yotta_assert(thread != 0);
    yotta_assert(socket_event != 0);
    yotta_assert(socket_event->socket_thread == thread);

    pthread_mutex_lock(&thread->mutex);
    {
        yotta_socket_event_t ** parent_ptr = &thread->socket_head;

        while (*parent_ptr != socket_event)
        {
            yotta_assert(*parent_ptr != 0);

            parent_ptr = &(*parent_ptr)->socket_next;
        }

        *parent_ptr = socket_event->socket_next;
    }
    pthread_mutex_unlock(&thread->mutex);

#ifdef YOTTA_DEBUG
    socket_event->socket_thread = 0;
#endif // YOTTA_DEBUG

    return 0;
}

uint64_t
yotta_socket_thread_destroy(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    thread->quit_status = YOTTA_SOCKET_THREAD_STOP_ON_EMPTY;

    if (pthread_join(thread->id, 0) != 0)
    {
        return -1;
    }

    pthread_mutex_destroy(&thread->mutex);

    yotta_assert(thread->socket_head == 0);

    return 0;
}

uint64_t
yotta_socket_thread_kill(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    thread->quit_status = YOTTA_SOCKET_THREAD_STOP_NOW;

    if (pthread_join(thread->id, 0) != 0)
    {
        return -1;
    }

    while (thread->socket_head)
    {
        yotta_socket_event_t * socket_event = thread->socket_head;

        thread->socket_head = socket_event->socket_next;

        yotta_socket_event_release(socket_event);
    }

    pthread_mutex_destroy(&thread->mutex);

    return 0;
}



uint64_t
yotta_socket_thread_plumbing_init(yotta_socket_thread_t * thread,
    yotta_thread_func_t func, yotta_thread_args_t args)
{
    yotta_assert(thread != NULL);
    yotta_assert(func != NULL);

    pthread_create(&thread->id, NULL, func, args);

    return YOTTA_SUCCESS;
}

uint64_t
yotta_socket_thread_join(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != NULL);

    pthread_join(thread->id, NULL);

    return YOTTA_SUCCESS;
}
