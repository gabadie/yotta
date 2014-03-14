
#include "testhelper_memory.h"
#include "testhelper_whisper.h"
#include "../src/whisper/yotta_whisper_shared_counter.private.h"


static
void
test_whisper_shared_counter()
{
    uint64_t const range_size = 16;
    yotta_counter_t counter = 0;
    yotta_shared_counter_t shared;

    { // test initialization
        testhelper_lorem_s(&shared);

        shared.master_counter_addr = yotta_addr(&counter);
        shared.range_size = range_size;
        shared.range_status = 0;
        shared.stock_size = range_size;

        test_assert(shared.range_size == shared.stock_size);
        test_assert(shared.range_size > 0);
    }

    testhelper_whisper_protocol_t protocol;
    testhelper_whisper_protocol_init(&protocol);

    { // protocol life expectency
        yotta_whisper_shared_counter(&protocol.queue1, &shared);

        sleep(1);
    }

    testhelper_whisper_protocol_destroy(&protocol);

    { // test end checking
        test_assert(counter == range_size);
    }

    testhelper_memory_check();
}

int
main()
{
    testhelper_init();
    testhelper_memory_setup();

    test_whisper_shared_counter();

    return 0;
}
