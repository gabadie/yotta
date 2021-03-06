#ifndef _YOTTAPRIVATE_WHISPER_LABELS
#define _YOTTAPRIVATE_WHISPER_LABELS

#include "../yotta_prefix.h"


/*
 * @infos: Enumeration listing all messages label in the whisper protocol, the
 *      the very one between the master and the slave.
 *
 * @asserts: Names must be sorted.
 */
enum
{
    YOTTA_WHISPER_COMMAND_FEEDBACK,
    YOTTA_WHISPER_COMMAND_ORDER,
    YOTTA_WHISPER_MEM_FETCH_ANSWER,
    YOTTA_WHISPER_MEM_FETCH_REQUEST,
    YOTTA_WHISPER_MEM_PUSH,
    YOTTA_WHISPER_SHARED_COUNTER_ANSWER,
    YOTTA_WHISPER_SHARED_COUNTER_REQUEST,
    YOTTA_WHISPER_STOP,
    YOTTA_WHISPER_LABELS_COUNT
};

/*
 * @infos: Enumeration type the whisper protocol is using.
 */
typedef uint8_t yotta_whisper_label_t;


#endif
