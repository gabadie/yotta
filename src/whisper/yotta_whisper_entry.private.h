#ifndef _YOTTAPRIVATE_WHISPER_ENTRY
#define _YOTTAPRIVATE_WHISPER_ENTRY

#include "../yotta_prefix.h"
#include "../socket/yotta_tcp.h"
#include "../yotta_context.h"


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
    // specify the number of bytes that has not been used by the wisper entry
    uint64_t unreaded;

    // specify the number of byte that the entry is still waiting for
    uint64_t remaining;
} yotta_whisper_entry_feedback_t;

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
    yotta_whisper_entry_feedback_t *,
    void *
);

#endif
