
#include <string.h>

//#include "testhelper_memory.h"
#include "testhelper_whisper.h"
#include "../src/whisper/yotta_whisper_command.private.h"

static
uint64_t thread_callback_signature = 0;

static
void
test_thread_callback(uint64_t * param)
{
    test_assert(*param == 9);
    test_assert(thread_callback_signature == 0);
    test_assert(yotta_memory_size(param) == sizeof(*param));

    (*param)++;
    thread_callback_signature++;

    test_assert(*param == 10);
}

static
void
test_whisper_command_invocation()
{
    testhelper_whisper_protocol_t protocol;

    testhelper_whisper_protocol_init(&protocol);

    {
        uint64_t param = 9;

        yotta_sync_t sync_sent;
        yotta_sync_t sync_finished;

        yotta_dirty_s(&sync_sent);
        yotta_dirty_s(&sync_finished);

        yotta_whisper_command(
            &protocol.queue0,
            (yotta_whisper_command_entry_t) test_thread_callback,
            sizeof(param),
            &param,
            0,
            1,
            0,
            1,
            &sync_sent,
            &sync_finished
        );

        test_assert(param == 9);

        test_assert(yotta_sync_wait(&sync_sent) == YOTTA_SUCCESS);

        test_assert(param == 9);

        test_assert(yotta_sync_wait(&sync_finished) == YOTTA_SUCCESS);

        test_assert(param == 9);
        test_assert(thread_callback_signature == 1);
    }

    testhelper_whisper_protocol_destroy(&protocol);

    // TODO: implements yotta_semaphore_pool_flush()
    //testhelper_memory_check();
}

int
main()
{
    testhelper_init();
    //testhelper_memory_setup();

    test_whisper_command_invocation();

    return 0;
}
