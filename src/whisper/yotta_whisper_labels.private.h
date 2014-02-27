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
    YOTTA_WHISPER_MEM_PULL_ANSWER,
    YOTTA_WHISPER_MEM_PULL_REQUEST,
    YOTTA_WHISPER_MEM_PUSH,
    YOTTA_WHISPER_LABELS_COUNT
};

/*
 * @infos: Enumeration type the whisper protocol is using.
 */
typedef uint8_t yotta_whisper_label_t;


#endif
