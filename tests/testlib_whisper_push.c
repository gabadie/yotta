
#include <string.h>

#include "testhelper_lorem.h"
#include "testhelper_memory.h"
#include "testhelper_whisper.h"
#include "../src/whisper/yotta_whisper_push.private.h"


static
void
test_whisper_push_stress()
{
    static uint64_t const data_size = 1024 * 1024;
    testhelper_whisper_protocol_t protocol;

    void * src_data = malloc(data_size);
    void * dest_data = malloc(data_size);

    testhelper_whisper_protocol_init(&protocol);
    testhelper_lorem(src_data, data_size);

    yotta_whisper_push(&protocol.queue0, (uint64_t) dest_data, data_size, src_data);

    testhelper_whisper_protocol_destroy(&protocol);

    test_assert(memcmp(src_data, dest_data, data_size) == 0);

    free(src_data);
    free(dest_data);

    testhelper_memory_check();
}

int
main()
{
    testhelper_init();
    testhelper_memory_setup();

    test_whisper_push_stress();

    return 0;
}
