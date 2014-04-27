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
#include <stdio.h>
#include <string.h>

#include "yotta_dictate_binary_send.private.h"
#include "yotta_dictate_labels.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.h"
#include "../core/yotta_logger.private.h"
#include "../socket/yotta_tcp.h"
#include "../threading/yotta_sync.private.h"
#include "../utils/yotta_file.h"

#define BINARY_BUFFER_SIZE 4096


typedef struct
yotta_dictate_binary_send_cmd_s
{
    // TCP command inheritance
    yotta_tcp_cmd_t abstract_cmd;

    // The header
    uint64_t header_cursor;
    struct
    {
        yotta_dictate_label_t label;
        uint64_t data_size;
    } __attribute__((packed))
    header;

    // File handler of the binary
    FILE * binary_file;

    // Cursor on the binary's data
    uint64_t data_cursor;

    // Last size of data read
    size_t data_read;

    // Temporary buffer for binary send
    uint8_t data[BINARY_BUFFER_SIZE];

    // The finish sync object
    yotta_sync_t * sync_finished;
}
yotta_dictate_binary_cmd_t;

/*
 * @infos: yotta_dictate binary release event
 */
static
void
yotta_dictate_binary_release(yotta_tcp_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);

    yotta_tcp_cmd_destroy(cmd);
    yotta_free(cmd);
}

/*
 * @infos: yotta_dictate binary send event
 */
static
void
yotta_dictate_binary_send(yotta_dictate_binary_cmd_t * cmd)
{
    yotta_assert(cmd != NULL);
    yotta_assert(cmd->abstract_cmd.queue != NULL);
    yotta_assert(cmd->binary_file != NULL);

    // Streams binary frame header
    yotta_tcp_cmd_stream(
        (yotta_tcp_cmd_t *) cmd,
        sizeof(cmd->header),
        &cmd->header_cursor,
        &cmd->header
    );

    // Streams the whole binary content in severals passes
    while (!feof(cmd->binary_file))
    {
        if (cmd->data_read == 0)
        {
            // Reads a new binary pass from the binary file
            cmd->data_read = fread(cmd->data, 1, BINARY_BUFFER_SIZE, cmd->binary_file);
            cmd->data_cursor = 0;
        }

        yotta_assert(cmd->data_read > 0);

        // Streams the current pass
        yotta_tcp_cmd_stream(
            (yotta_tcp_cmd_t *) cmd,
            cmd->data_read,
            &cmd->data_cursor,
            cmd->data
        );

        // Ends the current pass
        cmd->data_read = 0;
    }

    /*
     * We push the sync event and destroy this command
     */
    if (cmd->sync_finished)
    {
        yotta_sync_post(cmd->sync_finished);
    }

    yotta_tcp_cmd_finish((yotta_tcp_cmd_t *) cmd);

    fclose(cmd->binary_file);
    yotta_tcp_cmd_release(cmd);
}

static
yotta_tcp_cmd_vtable_t const
yotta_dictate_binary_send_vtable = {
    (yotta_tcp_cmd_entry_t) yotta_dictate_binary_release,
    (yotta_tcp_cmd_entry_t) yotta_dictate_binary_send
};

uint64_t
yotta_dictate_binary(
    yotta_dictate_queue_t * cmd_queue,
    char const * executable_path,
    yotta_sync_t * sync_finished
)
{
    yotta_assert(cmd_queue != NULL);

    // Initialize the finish sync object
    if (sync_finished != NULL)
    {
        yotta_sync_init(sync_finished);
    }

    /*
     * Create the yotta binary send command
     */
    yotta_dictate_binary_cmd_t * cmd = yotta_alloc_s(yotta_dictate_binary_cmd_t);

    yotta_dirty_s(cmd);

    // Initialize the command
    yotta_tcp_cmd_init(cmd, &yotta_dictate_binary_send_vtable);

    cmd->binary_file = fopen(executable_path, "rb");

    if(cmd->binary_file == NULL)
    {
        yotta_logger_error("Failed to open binary");
        return YOTTA_INVALID_VALUE;
    }

    cmd->header_cursor = 0;
    cmd->header.label = YOTTA_DICTATE_LABEL_SLAVE_BINARY;
    cmd->header.data_size = yotta_file_size(cmd->binary_file);
    cmd->data_read = 0;
    cmd->sync_finished = sync_finished;


    // Queue the command
    yotta_tcp_queue_append((yotta_tcp_queue_t *) cmd_queue, (yotta_tcp_cmd_t *) cmd);

    return YOTTA_SUCCESS;
}
