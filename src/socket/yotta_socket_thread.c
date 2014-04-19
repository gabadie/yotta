
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include "yotta_socket_thread.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_return.h"
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
    yotta_assert(thread->current_socket == 0);

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

        yotta_mutex_lock(&thread->mutex);
        {
            yotta_socket_event_t * socket_cursor = thread->socket_head;

            if (socket_cursor == 0 && thread->quit_status == YOTTA_SOCKET_THREAD_STOP_ON_EMPTY)
            {
                yotta_mutex_unlock(&thread->mutex);
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
        yotta_mutex_unlock(&thread->mutex);

        int32_t event_counts = select(fd_max + 1, &fd_set_recv, &fd_set_send, &fd_set_except, &timer);

        if (event_counts == -1)
        {
            if (errno == EBADF)
            {
                // bad file descriptor, a socket might have been destroyed -> we repoll
                continue;
            }

            yotta_logger_error("yotta_socket_thread_main: select error");

            yotta_assert(0);

            return 0;
        }

        yotta_mutex_lock(&thread->mutex);

        yotta_socket_event_t * socket_it = thread->socket_head;
        thread->current_socket = socket_it;

        while (event_counts != 0 && socket_it != 0)
        {
            int32_t fd = (int32_t)socket_it->socket.fd;

            yotta_mutex_unlock(&thread->mutex);

            if (FD_ISSET(fd, &fd_set_except))
            {
                yotta_assert(socket_it->except_event != 0);

                socket_it->except_event(socket_it);
                event_counts -= 1;

                if (FD_ISSET(fd, &fd_set_recv))
                {
                    event_counts -= 1;
                }

                if (FD_ISSET(fd, &fd_set_send))
                {
                    event_counts -= 1;
                }

                yotta_mutex_lock(&thread->mutex);

                /*
                 * Makes sure that the iterator has been changed because of its
                 * suicide
                 */
                yotta_assert(thread->current_socket != socket_it);

                socket_it = thread->current_socket;

                continue;
            }

            if (FD_ISSET(fd, &fd_set_recv))
            {
                yotta_assert(socket_it->recv_event != 0);

                socket_it->recv_event(socket_it);
                event_counts -= 1;

                /*
                 * Checks if the iterator has been changed because of a suicide
                 * in the receive event.
                 */
                if (thread->current_socket != socket_it)
                {
                    yotta_mutex_lock(&thread->mutex);

                    socket_it = thread->current_socket;

                    continue;
                }
            }

            if (FD_ISSET(fd, &fd_set_send))
            {
                yotta_assert(socket_it->send_event != 0);

                socket_it->send_event(socket_it);
                event_counts -= 1;

                /*
                 * Checks if the iterator has been changed because of a suicide
                 * in the receive event.
                 */
                if (thread->current_socket != socket_it)
                {
                    yotta_mutex_lock(&thread->mutex);

                    socket_it = thread->current_socket;

                    continue;
                }
            }

            yotta_mutex_lock(&thread->mutex);

            socket_it = socket_it->socket_next;
            thread->current_socket = socket_it;
        }

        thread->current_socket = 0;

        yotta_mutex_unlock(&thread->mutex);
    }

    return 0;
}

yotta_return_t
yotta_socket_thread_init(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    yotta_dirty_s(thread);

    if (yotta_mutex_init(&thread->mutex) != 0)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    thread->quit_status = YOTTA_SOCKET_THREAD_CONTINUE;
    thread->socket_event_count = 0;
    thread->socket_head = 0;
    thread->current_socket = 0;

    if (yotta_thread_create(&thread->id, yotta_socket_thread_main, thread) != 0)
    {
        yotta_mutex_destroy(&thread->mutex);
        return YOTTA_UNEXPECTED_FAIL;
    }

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_socket_thread_listen(yotta_socket_thread_t * thread, yotta_socket_event_t * socket_event)
{
    yotta_assert(thread != 0);
    yotta_assert(socket_event != 0);
    yotta_assert(socket_event->socket_thread == 0);
    yotta_assert(socket_event->except_event != 0);
    yotta_assert(socket_event->release_event != 0);

    socket_event->socket_thread = thread;

    yotta_mutex_lock(&thread->mutex);
    {
        if (thread->socket_event_count == (uint64_t)FD_SETSIZE)
        {
            yotta_mutex_unlock(&thread->mutex);

            return YOTTA_INVALID_OPERATION;
        }

        socket_event->socket_next = thread->socket_head;
        thread->socket_head = socket_event;
        thread->socket_event_count++;
    }
    yotta_mutex_unlock(&thread->mutex);

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_socket_thread_destroy(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    thread->quit_status = YOTTA_SOCKET_THREAD_STOP_ON_EMPTY;

    if (yotta_thread_join(thread->id) != 0)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    yotta_mutex_destroy(&thread->mutex);

    yotta_assert(thread->socket_head == 0);

    return YOTTA_SUCCESS;
}

yotta_return_t
yotta_socket_thread_kill(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    thread->quit_status = YOTTA_SOCKET_THREAD_STOP_NOW;

    if (yotta_thread_join(thread->id) != 0)
    {
        return YOTTA_UNEXPECTED_FAIL;
    }

    while (thread->socket_head)
    {
        yotta_socket_event_t * socket_event = thread->socket_head;

        thread->socket_head = socket_event->socket_next;

#ifdef YOTTA_ASSERT
        socket_event->socket_thread = 0;
#endif //YOTTA_ASSERT

        yotta_assert(socket_event->socket_thread == 0);

        yotta_socket_event_release(socket_event);
    }

    yotta_mutex_destroy(&thread->mutex);

    yotta_assert(thread->socket_head == 0);

    return YOTTA_SUCCESS;
}
