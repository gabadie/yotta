
#include <yotta.h>


/*
 * Slave argument function
 */
typedef struct massive_args_s
{
    uint64_t buffer_size;
    yotta_ptr_t buffer;
#ifdef _YOTTA_ITERATION_2
    yotta_atomic_counter_t counter;
#endif
} massive_args_t;

#ifdef _YOTTA_ITERATION_1
/*
 * Slave init entry point
 */
static
uint64_t
slave_startup_function(const massive_args_t * args)
{
    /*
     * This function is mono-threaded
     */

    /*
     * Returns 0 to let yotta launch all slave_massive_function instance on this slave. Return something else
     * would kill the slave execution on the current computer
     */
    return 0;
}
#endif

/*
 * Slave massive entry point
 */
static
void
slave_massive_function(const yotta_thread_t * thread, const massive_args_t * args)
{
    // id of the computer
    uint64_t computer_id = yotta_thread_computer_id(thread);

    // numbers of computers
    uint64_t computer_count = yotta_thread_computer_count(thread);

    // id of the thread on the computer
    uint64_t computer_thread_id = yotta_thread_computer_thread_id(thread);

    // numbers of thread on this computer
    uint64_t computer_thread_count = yotta_thread_computer_thread_count(thread);

    // global thread id
    uint64_t global_thread_id = yotta_thread_global_id(thread);

    // global thread count
    uint64_t global_thread_count = yotta_thread_global_count(thread);

    {
        /*
         * create an empty buffer mirror from the original buffer of the client, but without downloading (ideal for
         * write-only buffers)
         */
        uint64_t * buffer = (uint64_t *) yotta_pull_range(args->buffer, 0, 0);

#ifdef _YOTTA_ITERATION_2
        while ((uint64_t i = yotta_atomic_counter_add(&args->counter, 1)) < args->buffer_size)
        {
            buffer[i] = i;

            uint64_t offset = i * sizeof(uint64_t);
            uint64_t size = sizeof(uint64_t);

            // push a buffer range to the original buffer on the client
            yotta_push_range(buffer, offset, size); // implemented at the very begining
        }
#endif

        // release buffer mirror
        yotta_release(buffer);
    }

    {
        // clone the entire buffer from the client to a buffer mirror
        uint64_t * buffer = (uint64_t *) yotta_pull_range(args->buffer, 0, sizeof(uint64_t) * args->buffer_size);

        // release buffer mirror
        yotta_release(buffer);
    }

#ifdef _YOTTA_ITERATION_3
    {
        /*
         * Barrier with all threads on the same slave
         */
        yotta_group_barrier(thread);
    }
#endif

#ifdef _YOTTA_ITERATION_4
    {
        /*
         * Barrier with all threads on all slaves
         */
        yotta_global_barrier(thread);
    }
#endif

#ifdef _YOTTA_ITERATION_5
    {
        /*
         * Prints in the client std
         */
        yotta_global_printf("computer(%lu).thread(%lu)", computer_id, thread_id);
    }
#endif
}

#ifdef _YOTTA_ITERATION_1
/*
 * Slave finish entry point
 */
static
void
slave_finish_function(const massive_args_t * args)
{
    /*
     * This function is mono-threaded
     */
}
#endif

/*
 * Client main entry point (like a conventional program)
 */
int
main(int argc, char ** argv)
{
    yotta_init(argc, argv);


    yotta_context_t context;

    // connection to the yotta server that is going to run the slaves
    if (yotta_context_init(&context, "192.168.1.10", 5000) != 0)
    {
        return 1;
    }

    const uint64_t buffer_size = 1024;

    massive_args_t args;
    args.buffer_size = buffer_size;

    // alloc a yotta memory buffer with a 16 bytes alignement
    uint64_t * buffer = (uint64_t *) yotta_alloc(sizeof(uint64_t) * count, 16);

    {
        /*
         * Because a slave is executed in another process on another computer, pointers won't have same addresses
         * therefore the yotta library introduce the "yotta address" that is a virtual address mechanism.
         * you can create a yotta address from a pointer in the client, and then the create a mirror buffer from
         * the yotta address pointer on the yotta slave.
         */
        args.buffer = yotta_ptr(buffer);
    }

#ifdef _YOTTA_ITERATION_2
    {
        /*
         * Yotta's atomic counters are designed to be used across different processes, and even on different computers.
         * The client is holding the original atomic counter, and yotta slaves just increment it by IP based requests.
         * But for more performance, you might not want to have a IP based request for each increment. Therefore yotta
         * atomic counters are incremented by a given minimal range instead in the background to devide the requests
         * frequency.
         */

        uint64_t counter_init_value = 0;
        uint64_t counter_request_range = 16;

        yotta_atomic_counter_init(&args.counter, counter_init_value, counter_request_range);
    }
#endif

    {
        /*
         * A yotta command stores all callback pointers and arguemnts informations (size and pointers) mining that
         * modification after yotta_command_{init,startup,finish} will be considered
         */
        yotta_command_t command;
        yotta_command_init(&command, &slave_massive_function, &args, sizeof(args));
#ifdef _YOTTA_ITERATION_1
        yotta_command_startup(&command, &slave_startup_function, &args, sizeof(args));
        yotta_command_finish(&command, &slave_finish_function, &args, sizeof(args));
#endif

        // launch slaves
        if (yotta_command(&context, &command) != 0)
        {
            // failed to launch command
            return 1;
        }
    }

    // free a yotta memory buffer
    yotta_free(buffer);

    return 0;
}
