#ifndef _YOTTAPRIVATE_WHISPER_ENTRIES
#define _YOTTAPRIVATE_WHISPER_ENTRIES

#include "yotta_whisper_entry.private.h"


/*
 * @infos: processes a pull answer
 *
 * @params: see yotta_whisper_entry_t in yotta_whisper_entry.private.h
 */
void
yotta_whisper_entry_pull_answer(
    yotta_context_t * context,
    yotta_socket_t * socket,
    yotta_whisper_entry_feedback_t * feedback,
    void * tmp_buffer_ptr
);

/*
 * @infos: processes a pull request
 *
 * @params: see yotta_whisper_entry_t in yotta_whisper_entry.private.h
 */
void
yotta_whisper_entry_pull_request(
    yotta_context_t * context,
    yotta_socket_t * socket,
    yotta_whisper_entry_feedback_t * feedback,
    void * tmp_buffer_ptr
);

/*
 * @infos: processes a push
 *
 * @params: see yotta_whisper_entry_t in yotta_whisper_entry.private.h
 */
void
yotta_whisper_entry_push(
    yotta_context_t * context,
    yotta_socket_t * socket,
    yotta_whisper_entry_feedback_t * feedback,
    void * tmp_buffer_ptr
);

#endif
