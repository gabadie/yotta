
#include "yotta_slave_context.private.h"
#include "../core/yotta_debug.h"


/*
 * @infos: the global slave context
 */
yotta_slave_context_t * yotta_slave_context = 0;

/*
 * @infos: the global slave context without using dynamic allocation
 */
static
yotta_slave_context_t yotta_slave_context_plain;


uint64_t
yotta_slave_context_main(yotta_slave_parameters_t const * out_parameters)
{
    yotta_assert(out_parameters != 0);
    yotta_assert(out_parameters->client_ip != 0);
    yotta_assert(out_parameters->client_port != 0);
    yotta_assert(yotta_slave_context == 0);

    yotta_dirty_s(yotta_slave_context);

    if (yotta_socket_thread_init(&yotta_slave_context_plain.thread) != 0)
    {
        return -1;
    }

    uint64_t error = yotta_whisper_queue_connect(
        &yotta_slave_context_plain.queue,
        out_parameters->client_ip,
        out_parameters->client_port
    );

    if (error != 0)
    {
        yotta_socket_thread_destroy(&yotta_slave_context_plain.thread);
        return -1;
    }

    /*
     * Sets the slave queue's release event as the destroy event because
     * <yotta_slave_context_plain> it is statically allocated
     */
    yotta_socket_event_set_release(&yotta_slave_context_plain.queue, yotta_whisper_queue_destroy);

    /*
     * Publishes the slave context to the entire application
     */
    yotta_slave_context = &yotta_slave_context_plain;

    /*
     * Run the slave sockets' thread until the queue is released
     */
    yotta_socket_thread_destroy(&yotta_slave_context_plain.thread);

    return 0;
}
