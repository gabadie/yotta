/*
 *      This file is part of the Yotta Distributed Computing Library Project.
 *      Yotta Distributed Computing Library, named from the International
 *      System of Units (SI) denoting a factor of 10^24, is a C library
 *      designed for network-based distributed computing.
 *      Yotta provides features to the developer in order to execute
 *      simultaneously time-consuming algorithms across severals
 *      clustered computers.
 *
 *      Co-founders:
 *         Guillaume Abadie
 *         Thierry Cantenot
 *
 *      Yotta Distributed Computing Library's official code repository is
 *      available at <http://github.com/gabadie/yotta>.
 *
 *  GNU GPL v3 license:
 *      Yotta is a free C library: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  github.com:
 *      github.com is NEITHER property of the Yotta Distributed Computing
 *      Library Project NOR any of its Co-founders'.  Please see
 *      <http://github.com/site/terms>.
 */

#include "yotta_whisper_shared_counter.private.h"
#include "yotta_whisper_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../threading/yotta_atomic.h"


/*
 * @infos: defines a shared counter request command
 */
typedef struct
yotta_whisper_shared_counter_request_cmd_s
{
    // the TCP command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // the header
    uint64_t header_cursor;
    struct
    {
        yotta_whisper_label_t label;
        yotta_addr_t master_counter_addr;
        yotta_addr_t shared_counter_addr;
        uint64_t add;
    } __attribute__((packed))
    header;
}
yotta_whisper_shared_counter_request_cmd_t;

/*
 * @infos: defines a shared counter answer command
 */
typedef struct
yotta_whisper_shared_counter_answer_cmd_s
{
    // the tcp command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // the header
    uint64_t header_cursor;
    struct
    {
        yotta_whisper_label_t label;
        yotta_addr_t shared_counter_addr;
        uint64_t range;
    } __attribute__((packed))
    header;
}
yotta_whisper_shared_counter_answer_cmd_t;


/*
 * @infos: yotta_whisper_shared_counter_{answer,request}_cmd_t's release event
 */
static
void
yotta_whisper_shared_counter_release(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);

    yotta_tcp_cmd_destroy(cmd);
    yotta_free(cmd);
}

/*
 * @infos: yotta_whisper_shared_counter_request_cmd_t's send event
 */
static
void
yotta_whisper_shared_counter_request_send(yotta_whisper_shared_counter_request_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->abstract_cmd.queue != 0);

    // Streams shared counter request's header
    yotta_tcp_cmd_stream_unique(
        (yotta_tcp_cmd_t *) cmd,
        sizeof(cmd->header),
        &cmd->header_cursor,
        &cmd->header
    );

    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);
    yotta_tcp_cmd_release(cmd);
}

/*
 * @infos: yotta_whisper_shared_counter_answer_cmd_t's send event
 */
static
void
yotta_whisper_shared_counter_answer_send(yotta_whisper_shared_counter_answer_cmd_t * cmd)
{
    yotta_assert(cmd != 0);
    yotta_assert(cmd->abstract_cmd.queue != 0);

    // Streams shared counter answer's header
    yotta_tcp_cmd_stream_unique(
        (yotta_tcp_cmd_t *) cmd,
        sizeof(cmd->header),
        &cmd->header_cursor,
        &cmd->header
    );

    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);
    yotta_tcp_cmd_release(cmd);
}

static
yotta_tcp_cmd_vtable_t const
yotta_whisper_shared_counter_answer_vtable = {
    (yotta_tcp_cmd_entry_t) yotta_whisper_shared_counter_release,
    (yotta_tcp_cmd_entry_t) yotta_whisper_shared_counter_answer_send
};

/*
 * @infos: YOTTA_WHISPER_SHARED_COUNTER_REQUEST's receive event
 */
void
yotta_whisper_shared_counter_request_recv(
    yotta_whisper_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != 0);

    typedef struct
    yotta_whisper_buffer_s
    {
        uint64_t header_received;
        struct
        {
            yotta_addr_t master_counter_addr;
            yotta_addr_t shared_counter_addr;
            uint64_t add;
        }
        header;
    }
    yotta_whisper_buffer_t;

    yotta_whisper_buffer_t * buffer =
        yotta_whisper_queue_recv_buffer(cmd_queue, yotta_whisper_buffer_t);

    {
        // we are receiving the shared counter request's header

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->header),
            &buffer->header_received,
            &buffer->header
        );

        if (op != 0)
        {
            return;
        }
    }

    {
        // generates shared counter answer command

        yotta_whisper_shared_counter_answer_cmd_t * cmd =
            yotta_alloc_s(yotta_whisper_shared_counter_answer_cmd_t);

        yotta_dirty_s(cmd);
        yotta_tcp_cmd_init(cmd, &yotta_whisper_shared_counter_answer_vtable);

        cmd->header_cursor = 0;
        cmd->header.label = YOTTA_WHISPER_SHARED_COUNTER_ANSWER;
        cmd->header.shared_counter_addr = buffer->header.shared_counter_addr;

        yotta_counter_t * counter = (yotta_counter_t *) buffer->header.master_counter_addr;

        cmd->header.range = yotta_atomic_fetch_add(counter, (yotta_counter_t) buffer->header.add);

        yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
    }

    // we clean up the tmp buffer
    buffer->header_received = 0;
    buffer->header.master_counter_addr = 0;
    buffer->header.shared_counter_addr = 0;
    buffer->header.add = 0;

    yotta_whisper_queue_finish(cmd_queue);
}

/*
 * @infos: YOTTA_WHISPER_SHARED_COUNTER_ANSWER's receive event
 */
void
yotta_whisper_shared_counter_answer_recv(
    yotta_whisper_queue_t * cmd_queue
)
{
    yotta_assert(cmd_queue != 0);

    typedef struct
    yotta_whisper_buffer_s
    {
        uint64_t header_cursor;
        struct
        {
            yotta_addr_t shared_counter_addr;
            uint64_t range;
        }
        header;

        uint64_t data_cursor;
    }
    yotta_whisper_buffer_t;

    yotta_whisper_buffer_t * buffer =
        yotta_whisper_queue_recv_buffer(cmd_queue, yotta_whisper_buffer_t);

    {
        // we are receiving the shared counter awnser's header

        uint64_t op = yotta_tcp_queue_recv(
            (yotta_tcp_queue_t *) cmd_queue,
            sizeof(buffer->header),
            &buffer->header_cursor,
            &buffer->header
        );

        if (op != 0)
        {
            return;
        }
    }

    {
        yotta_shared_counter_t * shared =
            (yotta_shared_counter_t *) buffer->header.shared_counter_addr;

        yotta_assert(shared->range_status != 0x3);

        /*
         * First, we assume this is only the second range
         */
        shared->range_start[1] = buffer->header.range;

        if (!yotta_atomic_compare_swap_b(&shared->range_status, 0x1, 0x3))
        {
            /*
             * Its has failed, so this is the first one
             */
            shared->range_start[0] = buffer->header.range;
            shared->range_status = 0x1;
        }
    }

    buffer->header_cursor = 0;
    buffer->header.shared_counter_addr = 0;
    buffer->header.range = 0;

    yotta_whisper_queue_finish(cmd_queue);
}

static
yotta_tcp_cmd_vtable_t const
yotta_whisper_shared_counter_request_vtable = {
    (yotta_tcp_cmd_entry_t) yotta_whisper_shared_counter_release,
    (yotta_tcp_cmd_entry_t) yotta_whisper_shared_counter_request_send
};

void
yotta_whisper_shared_counter(
    yotta_whisper_queue_t * cmd_queue,
    yotta_shared_counter_t * shared_counter
)
{
    yotta_assert(cmd_queue != 0);
    yotta_assert(shared_counter != 0);
    yotta_assert(shared_counter->master_counter_addr != 0);
    yotta_assert(shared_counter->range_status <= 0x1);
    yotta_assert(shared_counter->range_size != 0);

    /*
     * Creates the YOTTA_WHISPER_SHARED_COUNTER_REQUEST command
     */
    yotta_whisper_shared_counter_request_cmd_t * cmd =
        yotta_alloc_s(yotta_whisper_shared_counter_request_cmd_t);

    yotta_dirty_s(cmd);
    yotta_tcp_cmd_init(cmd, &yotta_whisper_shared_counter_request_vtable);

    cmd->header_cursor = 0;
    cmd->header.label = YOTTA_WHISPER_SHARED_COUNTER_REQUEST;
    cmd->header.master_counter_addr = shared_counter->master_counter_addr;
    cmd->header.shared_counter_addr = (yotta_addr_t) shared_counter;
    cmd->header.add = shared_counter->stock_size;

    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);
}
