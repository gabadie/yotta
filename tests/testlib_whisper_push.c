
#include <string.h>

#include "testhelper_tcp_sockets.h"
#include "testhelper_whisper.h"
#include "testhelper_lorem.h"
#include "../src/whisper/yotta_whisper_push.private.h"
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

#if 0
static
void
test_whisper_push()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);

    {
        test_assert(sizeof(testlib_whisper_push_frame_t) == 2 + 2 * 8 + CONTENT_SIZE);

        yotta_whisper_queue_t queue;

        memcpy(&queue, &sockets.sending_socket, sizeof(yotta_socket_t));

        testlib_whisper_push_frame_t sending;
        testlib_whisper_push_frame_t receiving;

        sending.label = (uint16_t) YOTTA_WHISPER_MEM_PUSH;
        sending.master_address = (uint64_t)&receiving;
        sending.data_size = CONTENT_SIZE;
        strcpy(sending.data_content, "hello !");

        yotta_whisper_push(
            &queue,
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
#endif

#if 0
static
void
test_whisper_push_entry()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);

    {
        char const * data_msg  = "hello world!";
        uint64_t const data_msg_len = strlen(data_msg) + 1;
        uint64_t const data_msg_cut = 4;
        char dest_buffer[CONTENT_SIZE];
        uint64_t dest_buffer_addr = (uint64_t) dest_buffer;

        yotta_whisper_entry_feedback_t feedback_cleaned;

        yotta_whisper_feedback_init(&feedback_cleaned);

        { // tests full compatibility with yotta_whisper_push()
            yotta_whisper_entry_feedback_t feedback;
            yotta_whisper_feedback_init(&feedback);

            yotta_whisper_push(
                &sockets.sending_socket,
                dest_buffer_addr,
                data_msg_len,
                (void const *) data_msg
            );

            test_assert(yotta_tcp_seek(&sockets.client_socket, 2) == 2);

            yotta_whisper_entry_push(0, &sockets.client_socket, &feedback);

            test_assert(strcmp(data_msg, dest_buffer) == 0);
            test_assert(memcmp(&feedback, &feedback_cleaned, sizeof(feedback)) == 0);
        }

        { // tests small package sending
            yotta_whisper_entry_feedback_t feedback;
            yotta_whisper_feedback_init(&feedback);

            yotta_tcp_send(&sockets.sending_socket, &dest_buffer_addr, sizeof(dest_buffer_addr));
            yotta_whisper_entry_push(0, &sockets.client_socket, &feedback);

            yotta_tcp_send(&sockets.sending_socket, &data_msg_len, sizeof(data_msg_len));
            yotta_socket_nonblock(&sockets.client_socket);
            yotta_whisper_entry_push(0, &sockets.client_socket, &feedback);
            yotta_socket_block(&sockets.client_socket);

            yotta_tcp_send(&sockets.sending_socket, data_msg, data_msg_cut);
            yotta_whisper_entry_push(0, &sockets.client_socket, &feedback);

            yotta_tcp_send(&sockets.sending_socket, data_msg + data_msg_cut, data_msg_len - data_msg_cut);
            yotta_whisper_entry_push(0, &sockets.client_socket, &feedback);

            test_assert(strcmp(data_msg, dest_buffer) == 0);
            test_assert(memcmp(&feedback, &feedback_cleaned, sizeof(feedback)) == 0);
        }
    }

    testhelper_tcp_clean(&sockets);
}
#endif

#if 0 // TODO: must implements protocol feeding in another thread to pass this test

static
void
test_whisper_push_stress()
{
    testhelper_tcp_sockets_t sockets;

    testhelper_tcp_build(&sockets);

    {
        static uint64_t const data_size = 1024 * 1024;// 1024 * 1024 * 1;

        void * src_data = malloc(data_size);
        void * dest_data = malloc(data_size);

        testhelper_lorem(src_data, data_size);

        yotta_whisper_entry_feedback_t feedback;
        yotta_whisper_feedback_init(&feedback);

        yotta_whisper_push(
            &sockets.sending_socket,
            (uint64_t) dest_data,
            data_size,
            src_data
        );

        test_assert(yotta_tcp_seek(&sockets.client_socket, 2) == 2);

        yotta_whisper_entry_push(0, &sockets.client_socket, &feedback);

        test_assert(strcmp(src_data, dest_data) == 0);

        free(src_data);
        free(dest_data);
    }

    testhelper_tcp_clean(&sockets);
}

#endif

static
void
test_whisper_push_stress()
{
    static uint64_t const data_size = 1024 * 1024;// 1024 * 1024 * 1;
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
}

int
main()
{
    testhelper_init();

    test_whisper_push_stress();

    return 0;
}
