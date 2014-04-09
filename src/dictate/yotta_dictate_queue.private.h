#ifndef _YOTTAPRIVATE_DICTATE_QUEUE
#define _YOTTAPRIVATE_DICTATE_QUEUE

#include "../socket/yotta_tcp_queue.private.h"
#include "yotta_dictate_labels.private.h"

/*
 * @infos: defines the sizes of the receive buffer in bytes
 */
#define YOTTA_DICTATE_RECV_BUFFER_SIZE 512

/*
 *@infos: yotta_dictate_queue struct's forward declaration
 */
typedef struct
yotta_dictate_queue_s
yotta_dictate_queue_t;

/*
 * @infos: defines the yotta dictate entry pointer type
 *
 * The entry receive 1 parameter:
 *  - yotta_dictate_queue_t * <cmd_queue>: the receiving command queue
 */
typedef void (* yotta_dictate_recv_t)(
    yotta_dictate_queue_t *
);

/*
 * yotta_dictate_queue_t's receiving virtual table
 */
typedef struct
yotta_dictate_vtable_s
{
    // YOTTA_DICTATE_LABEL_DEAMON_INFO's entry point
    void (* receive_daemon_info)(uint64_t, uint64_t);

    // YOTTA_DICTATE_LABEL_DEAMON_ERROR's entry point
    void (* receive_daemon_error)(char const *);
}
yotta_dictate_vtable_t;

/*
 * @infos: defines a dictate commands queue
 *
 * Layout of a frame:
 * |  2   |     8     | xxxx|
 * |label | data_size | data|
 */
struct
yotta_dictate_queue_s
{
    // Inheritance from TCP queue
    yotta_tcp_queue_t tcp_queue;

    // Callback entry to complete the message
    yotta_dictate_recv_t callback;

    // Frame header
    struct
    {
        yotta_dictate_label_t label;
        uint64_t data_size;
    } __attribute__((packed))
    header;

    uint64_t header_cursor; // Header cursor

    // Temporary buffer
    uint8_t recv_buffer[YOTTA_DICTATE_RECV_BUFFER_SIZE];

    // Receiving virtual table
    yotta_dictate_vtable_t const * vtable;
};

/*
 * @infos: initializes a dictate command queue
 *
 * @param <cmd_queue>: the command queue to initialize
 */
void
yotta_dictate_queue_init(yotta_dictate_queue_t * cmd_queue);

/*
 * @infos: connects a dictate command queue to a dictate master
 *
 * @param <cmd_queue>: the command queue to connect to
 * @param <ip>: the dictate master' ip
 * @param <port>: the dictate master' port
 *
 * @returns:
 *  == <0> if succeed
 *  != <0> if failed
 */
yotta_return_t
yotta_dictate_queue_connect(yotta_dictate_queue_t * cmd_queue, char const * ip, uint16_t port);

/*
 * @infos: destroyes a dictate command queue
 *
 * @param <cmd_queue>: the command queue to destroy
 */
void
yotta_dictate_queue_destroy(yotta_dictate_queue_t * cmd_queue);

/*
 * @infos: gets the receive buffer
 *
 * @param <cmd_queue>: the command queue
 * @param <buffer_type>: the receive buffer struct to cast to
 *
 * @return:
 *  (<buffer_type> *) receive buffer
 */
#define yotta_dictate_queue_recv_buffer(cmd_queue,buffer_type) \
    (buffer_type *) (cmd_queue)->recv_buffer ; \
    yotta_assert(sizeof(buffer_type) <= YOTTA_DICTATE_RECV_BUFFER_SIZE)

/*
 * @infos: finish  in the <cmd_queue> the number of unreaded bytes
 *
 * @param <cmd_queue>: the command queue
 */
#define yotta_dictate_queue_finish(cmd_queue) \
    (cmd_queue)->callback = NULL;

#endif
