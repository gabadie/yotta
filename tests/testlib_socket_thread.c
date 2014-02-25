
#include <string.h>

#include "testhelper_whisper.h"


static
void
test_socket_thread_destroy()
{
    testhelper_whisper_protocol_t protocol;

    testhelper_whisper_protocol_init(&protocol);
    {

    }
    testhelper_whisper_protocol_destroy(&protocol);
}

int
main()
{
    testhelper_init();

    test_socket_thread_destroy();

    return 0;
}
