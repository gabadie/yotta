#ifndef _YOTTAPRIVATE_DICTATE_LABELS
#define _YOTTAPRIVATE_DICTATE_LABELS

/*
 * @infos: Listing of all label messages in the Dictate protocol
 * (between Master and Daemon)
 */

/* Daemon errors */
#define YOTTA_DICTATE_LABEL_DAEMON_ERROR 0x0000

 /* Daemon to Master */
#define YOTTA_DICTATE_LABEL_DAEMON_INFO  0x1000

 /* Master to Daemon */
#define YOTTA_DICTATE_LABEL_SLAVE_BINARY 0x2000
#define YOTTA_DICTATE_LABEL_DAEMON_START 0x2010

/*
 * @infos: Type of the Dictate protocol's labels
 */
typedef uint16_t yotta_dictate_label_t;

#endif //_YOTTAPRIVATE_DICTATE_LABELS
