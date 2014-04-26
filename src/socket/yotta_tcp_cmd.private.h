#ifndef _YOTTAPRIVATE_TCP_CMD
#define _YOTTAPRIVATE_TCP_CMD

#include "yotta_socket_event.h"


/*
 * @infos: defines yotta tcp command entry point function pointer type
 *
 * The tcp command entry received 2 parameters:
 *  - yotta_tcp_cmd_t cmd: the tcp_cmd
 */
typedef void (*yotta_tcp_cmd_entry_t)(yotta_tcp_cmd_t *);

typedef struct
yotta_tcp_cmd_vtable_s
{
    // the yotta_tcp_cmd_t's release function
    yotta_tcp_cmd_entry_t release;

    // Command's stream event called by the TCP queue's stream event
    yotta_tcp_cmd_entry_t stream;
}
yotta_tcp_cmd_vtable_t;

/*
 * @infos: defines a TCP command to be inherited
 */
struct
yotta_tcp_cmd_s
{
    // the send event entry point
    yotta_tcp_cmd_vtable_t const * const vtable;

    // the next TCP command in the queue
    yotta_tcp_cmd_t * queue_next;

    // the parent TCP command queue
    yotta_tcp_queue_t * queue;
};

/*
 * @infos: inits the TCP command
 *
 * @param <cmd>: the socket event
 */
#ifdef YOTTA_ASSERT
#define yotta_tcp_cmd_init(cmd, virtual_table) \
    {\
        yotta_dirty_s((yotta_tcp_cmd_t *) (cmd)); \
        ((yotta_tcp_cmd_t *) (cmd))->queue = NULL; \
        yotta_vtable_set(cmd, virtual_table); \
    }

#else
#define yotta_tcp_cmd_init(cmd, virtual_table) \
    { \
        yotta_dirty_s((yotta_tcp_cmd_t *) (cmd)); \
        yotta_vtable_set(cmd, virtual_table); \
    }

#endif // YOTTA_ASSERT

/*
 * @infos: gets command's queue
 */
#define yotta_tcp_cmd_queue(cmd) \
    ((yotta_tcp_cmd_t *) (cmd))->queue

/*
 * @infos: sends the buffer through the TCP command's queue
 *
 * @param <cmd>: the TCP command
 * @param <buffer_size>: the buffer's size to send
 * @param <buffer_cursor>: the buffer's sending cursor
 * @param <buffer>: the buffer to send
 *
 * @returns
 *  - <0> when *buffer_cursor == buffer_size
 *  - <1> if *buffer_cursor < buffer_size
 */
uint64_t
yotta_tcp_cmd_send(yotta_tcp_cmd_t * cmd, uint64_t buffer_size, uint64_t * buffer_cursor, void const * buffer);

/*
 * @plumbing: Use yotta_tcp_cmd_stream() instead.
 */
#define _yotta_tcp_cmd_stream(cmd, buffer_size, buffer_cursor, buffer) \
    { \
        if ( \
            yotta_tcp_cmd_send( \
                (yotta_tcp_cmd_t *) (cmd), \
                (buffer_size), \
                (buffer_cursor), \
                (buffer) \
            ) != 0 \
        ) \
        { \
            return; \
        } \
    }

/*
 * Streams a command's output and automatically returns the command if the
 * memory block is not sent completly. This shall only be called in command's
 * stream virtual function (see yotta_tcp_cmd_vtable_t::stream).
 *
 * @param <cmd>: the TCP command
 * @param <buffer_size>: the buffer's size to send
 * @param <buffer_cursor>: the buffer's sending cursor
 * @param <buffer>: the buffer to send
 */
#define yotta_tcp_cmd_stream(cmd, buffer_size, buffer_cursor, buffer) \
    if ((buffer_size) != *(buffer_cursor)) \
    { \
        _yotta_tcp_cmd_stream(cmd, buffer_size, buffer_cursor, buffer); \
    }

/*
 * Same as yotta_tcp_cmd_stream(), but stream the last command's memory block
 *
 * @example:
 *  yotta_tcp_cmd_stream(cmd, [block 0]);
 *  yotta_tcp_cmd_stream(cmd, [block 1]);
 *  yotta_tcp_cmd_stream(cmd, [block 2]);
 *  yotta_tcp_cmd_stream_last(cmd, [block 3]);
 */
#define yotta_tcp_cmd_stream_last(cmd, buffer_size, buffer_cursor, buffer) \
    _yotta_tcp_cmd_stream(cmd, buffer_size, buffer_cursor, buffer);

/*
 * Same as yotta_tcp_cmd_stream(), but stream the unique command's memory block
 */
#define yotta_tcp_cmd_stream_unique(cmd, buffer_size, buffer_cursor, buffer) \
    _yotta_tcp_cmd_stream(cmd, buffer_size, buffer_cursor, buffer);

/*
 * @threadsafe
 * @infos: finishes the command
 *
 * @param <cmd>: the command to finish
 */
void
yotta_tcp_cmd_finish(yotta_tcp_cmd_t * cmd);

/*
 * @infos: triggers the release event
 *
 * @param <cmd>: the command to release
 */
#define yotta_tcp_cmd_release(cmd) \
    yotta_vcall((yotta_tcp_cmd_t *) cmd, yotta_tcp_cmd_vtable_t, release)

/*
 * @infos: destroy the TCP command
 *
 * @param <cmd>: the command to destroy
 */
#ifdef YOTTA_ASSERT
void
yotta_tcp_cmd_destroy(yotta_tcp_cmd_t * cmd);

#else
# define yotta_tcp_cmd_destroy(cmd)
    /*
     * We do nothing to make sur that -Wunused-params detects that the command is
     * beging freed right after.
     */

#endif //YOTTA_ASSERT



#endif
