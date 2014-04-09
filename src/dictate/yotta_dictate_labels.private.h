#ifndef _YOTTAPRIVATE_DICTATE_LABELS
#define _YOTTAPRIVATE_DICTATE_LABELS

/*
 * @infos: Listing of all label messages in the Dictate protocol
 * (between Master and Daemon)
 */

 /* Daemon to Master */
#define YOTTA_DICTATE_LABEL_DEAMON_INFO 0x1000

 /* Master to Daemon */
#define YOTTA_DICTATE_LABEL_SLAVE_BINARY 0x2000

/* Errors */
#define YOTTA_DICTATE_LABEL_ERROR 0x3000


/*
 * @infos: Type of the Dictate protocol's labels
 */
typedef uint16_t yotta_dictate_label_t;

#endif //_YOTTAPRIVATE_DICTATE_LABELS
