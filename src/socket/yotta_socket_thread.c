
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "yotta_socket_thread.h"
#include "../yotta_debug.h"
#include "../yotta_return.h"
#include "../yotta_logger.private.h"


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

    timer.tv_sec = 1;
    timer.tv_usec = 0;

    while (thread->quit_status != YOTTA_SOCKET_THREAD_STOP_NOW)
    {
        int32_t fd_max = 0;

        FD_ZERO(&fd_set_recv);
        FD_ZERO(&fd_set_send);
        FD_ZERO(&fd_set_except);

        yotta_socket_event_t * socket_head = thread->socket_head;

        if (socket_head == 0 && thread->quit_status == YOTTA_SOCKET_THREAD_STOP_ON_EMPTY)
        {
            break;
        }

        {
            yotta_socket_event_t * socket_cursor = socket_head;

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

        int32_t event_counts = select(fd_max + 1, &fd_set_recv, &fd_set_send, &fd_set_except, &timer);

        if (event_counts == -1)
        {
            yotta_logger_error("yotta_socket_thread_main: select error");

            return 0;
        }

        while (event_counts != 0)
        {
            yotta_assert(socket_head != 0);

            yotta_socket_event_t * socket_event = socket_head;

            /*
             * we move to the next socket event because the current one might be
             * destroyed in the exception callback
             */
            socket_head = socket_head->socket_next;

            int32_t fd = (int32_t)socket_event->socket.fd;

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
        }
    }

    return 0;
}

uint64_t
yotta_socket_thread_init(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    thread->socket_head = 0;
    thread->quit_status = YOTTA_SOCKET_THREAD_CONTINUE;

    return pthread_create(&thread->id, 0, (void *(*)(void *))yotta_socket_thread_main, thread);
}

uint64_t
yotta_socket_thread_listen(yotta_socket_thread_t * thread, yotta_socket_event_t * socket_event)
{
    yotta_assert(thread != 0);
    yotta_assert(socket_event != 0);

    do
    {
        socket_event->socket_next = thread->socket_head;
    }
    while (!__sync_bool_compare_and_swap(&thread->socket_head, socket_event->socket_next, socket_event));

    return 0;
}

uint64_t
yotta_socket_thread_destroy(yotta_socket_thread_t * thread)
{
    yotta_assert(thread != 0);

    thread->quit_status = YOTTA_SOCKET_THREAD_STOP_NOW;

    return pthread_join(thread->id, 0);
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
