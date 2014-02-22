#ifndef _YOTTAPRIVATE_WHISPER_ENTRY
#define _YOTTAPRIVATE_WHISPER_ENTRY

#include "../yotta_prefix.h"
#include "../socket/yotta_tcp.h"
#include "../yotta_context.h"


/*
 *@infos: yotta_whisper_entry_feedback_t's forward declaration
 */
typedef struct
yotta_whisper_entry_feedback_s
yotta_whisper_entry_feedback_t;

/*
 * @infos: defines the yotta wisper entry pointe type
 *
 * The logger entry received 3 parameters:
 *  - yotta_context_t * <context>: the bound context
 *  - yotta_socket_t * <socket>: the receiving socket
 *  - yotta_wisper_entry_feedback_t * <remaining>: the entry feedback
 *  - void * <tmp_buffer_ptr>: a pointer to a temporary buffer
 */
typedef void (* yotta_whisper_entry_t)(
    yotta_context_t *,
    yotta_socket_t *,
    yotta_whisper_entry_feedback_t *
);

/*
 * @infos: defines the sizes of the temprorary buffer in bytes
 */
#define YOTTA_WHISPER_TMP_BUFFER_SIZE 512


/*
 * @infos: groups wisper entry point feedback
 */
typedef struct
yotta_whisper_entry_feedback_s
{
    // callback entry to complete the message
    yotta_whisper_entry_t callback;

    // temporary buffer
    uint8_t buffer[YOTTA_WHISPER_TMP_BUFFER_SIZE];
} yotta_whisper_entry_feedback_t;

/*
 * @infos: gets the temporary buffer
 *
 * @param <feedback>: the entry's feedback ptr
 * @param <buffer_type>: the tmp buffer struct to cast to
 *
 * @return:
 *  (<buffer_type> *) tmp buffer
 */
#define yotta_whisper_tmp_buffer(feedback,buffer_type) \
    (buffer_type *) (feedback)->buffer ; \
    yotta_assert(sizeof(buffer_type) <= YOTTA_WHISPER_TMP_BUFFER_SIZE)

/*
 * @infos: sets in the <feedback> the number of unreaded bytes
 *
 * @param <feedback>: the entry's feedback ptr
 */
#define yotta_whisper_feedback_finish(feedback) \
    (feedback)->callback = 0;

/*
 * @infos: sets in the <feedback>'s <callback> to process futut incomming data
 *
 * @param <feedback>: the entry's feedback ptr
 * @param <callback_func_ptr>: the callback to call when receiving some data
 */
#define yotta_whisper_feedback_continue(feedback,callback_func_ptr) \
    (feedback)->callback = (callback_func_ptr);

#endif
