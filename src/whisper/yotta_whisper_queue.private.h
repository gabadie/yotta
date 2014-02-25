#ifndef _YOTTAPRIVATE_WHISPER_QUEUE
#define _YOTTAPRIVATE_WHISPER_QUEUE

#include "../socket/yotta_tcp_cmd_queue.private.h"


/*
 * @infos: defines the sizes of the receive buffer in bytes
 */
#define YOTTA_WHISPER_RECV_BUFFER_SIZE 512

/*
 *@infos: yotta_whisper_entry_feedback_t's forward declaration
 */
typedef struct
yotta_whisper_queue_s
yotta_whisper_queue_t;

/*
 * @infos: defines the yotta wisper entry pointe type
 *
 * The logger entry received 3 parameters:
 *  - yotta_whisper_queue_t * <cmd_queue>: the receiving command queue
 */
typedef void (* yotta_whisper_recv_t)(
    yotta_whisper_queue_t *
);

/*
 * @infos: defines a whisper commands queue
 */
struct
yotta_whisper_queue_s
{
    yotta_tcp_cmd_queue_t tcp_queue;

    // callback entry to complete the message
    yotta_whisper_recv_t callback;

    // temporary buffer
    uint8_t recv_buffer[YOTTA_WHISPER_RECV_BUFFER_SIZE];
};

/*
 * @infos: initializes a whisper command queue
 *
 * @param <cmd_queue>: the command queue to initialize
 */
void
yotta_whisper_queue_init(yotta_whisper_queue_t * cmd_queue);

/*
 * @infos: destroyes a whisper command queue
 *
 * @param <cmd_queue>: the command queue to destroy
 */
void
yotta_whisper_queue_destroy(yotta_whisper_queue_t * cmd_queue);


#endif
