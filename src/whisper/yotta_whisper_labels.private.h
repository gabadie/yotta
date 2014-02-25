#ifndef _YOTTAPRIVATE_WHISPER_LABELS
#define _YOTTAPRIVATE_WHISPER_LABELS

/*
 * @infos: Enumeration listing all messages label in the whisper protocol, the
 *      the very one between the master and the slave.
 *
 * @asserts: Names must be sorted.
 */
typedef enum
{
    YOTTA_WHISPER_MEM_PULL_ANSWER,
    YOTTA_WHISPER_MEM_PULL_REQUEST,
    YOTTA_WHISPER_MEM_PUSH
} yotta_whisper_trame_t;

#endif
