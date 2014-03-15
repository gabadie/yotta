
#include "yotta_socket_event.h"
#include "yotta_socket_thread.h"
#include "../core/yotta_debug.h"


void
yotta_socket_event_unlisten(yotta_socket_event_t * socket_event)
{
    yotta_assert(socket_event != 0);
    yotta_assert(socket_event->socket_thread != 0);

    yotta_socket_thread_t * thread = socket_event->socket_thread;

    yotta_mutex_lock(&thread->mutex);
    {
        yotta_assert(thread->socket_event_count > 0);

        yotta_socket_event_t ** parent_ptr = &thread->socket_head;

        while (*parent_ptr != socket_event)
        {
            yotta_assert(*parent_ptr != 0);

            parent_ptr = &(*parent_ptr)->socket_next;
        }

        *parent_ptr = socket_event->socket_next;

        if (thread->current_socket != 0)
        {
            /*
             * the socket thread might be currently working on excatly on this
             * socket event. In this case, we have to change it's iterator
             */
            if (thread->current_socket == socket_event)
            {
                thread->current_socket = socket_event->socket_next;
            }
        }

        thread->socket_event_count--;
    }
    yotta_mutex_unlock(&thread->mutex);

#ifdef YOTTA_ASSERT
    socket_event->socket_thread = 0;
#endif //YOTTA_ASSERT
}
