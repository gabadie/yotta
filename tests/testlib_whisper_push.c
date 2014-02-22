
#include <string.h>

#include "testhelper_tcp_sockets.h"
#include "../src/whisper/yotta_whisper_commands.h"
#include "../src/whisper/yotta_whisper_labels.private.h"


#define CONTENT_SIZE 512

typedef struct
testlib_whisper_push_frame_s
{
    uint16_t label;
    uint64_t master_address;
    uint64_t data_size;
    char data_content[CONTENT_SIZE];
} __attribute__((packed))
testlib_whisper_push_frame_t;

static
void
test_whisper_push()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);

    {
        test_assert(sizeof(testlib_whisper_push_frame_t) == 2 + 2 * 8 + CONTENT_SIZE);

        testlib_whisper_push_frame_t sending;
        testlib_whisper_push_frame_t receiving;

        sending.label = (uint16_t) YOTTA_WHISPER_MEM_PUSH;
        sending.master_address = (uint64_t)&receiving;
        sending.data_size = CONTENT_SIZE;
        strcpy(sending.data_content, "hello !");

        yotta_whisper_push(
            &sockets.sending_socket,
            sending.master_address,
            sending.data_size,
            sending.data_content
        );

        uint64_t received_bytes = yotta_tcp_recvall(
            &sockets.client_socket,
            &receiving,
            sizeof(receiving)
        );

        test_assert(received_bytes == sizeof(testlib_whisper_push_frame_t));

        test_assert(memcmp(&sending, &receiving, sizeof(testlib_whisper_push_frame_t)) == 0);
    }

    testhelper_tcp_clean(&sockets);
}

int
main(int argc, char const * const * argv)
{
    yotta_init(argc, argv);

    test_whisper_push();

    return 0;
}
