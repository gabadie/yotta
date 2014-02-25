
#include <string.h>

#include "testhelper_whisper.h"


static
void
test_socket_thread_destroy()
{
    testhelper_whisper_protocol_t protocol;

    testhelper_whisper_protocol_init(&protocol);
    testhelper_whisper_protocol_destroy(&protocol);
}

static
void
test_socket_thread_kill()
{
    testhelper_whisper_protocol_t protocol;

    testhelper_whisper_protocol_init(&protocol);
    yotta_socket_thread_kill(&protocol.thread);
}

int
main()
{
    testhelper_init();

    test_socket_thread_destroy();
    test_socket_thread_kill();

    return 0;
}
