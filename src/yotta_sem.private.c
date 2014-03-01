#include <string.h>
#include <pthread.h>

#include "core/yotta_debug.h"
#include "core/yotta_logger.private.h"
#include "core/yotta_memory.h"
#include "core/yotta_return.h"
#include "yotta_sem.private.h"

#define YOTTA_SEM_CHUNK_SIZE 64

typedef struct
yotta_sem_s
yotta_sem_t;

/*
 * @infos Chunk of a deque of semaphores
 */
struct
yotta_sem_s
{
    sem_t sem[64];      // List of POSIX semaphores
    uint64_t used;      // Bitfield used to check if a semaphore is in use
    yotta_sem_t * next; // Pointer to the next semaphores chunk
};

// Semaphores pool used for thread synchronisation
static
yotta_sem_t * sem_pool = NULL;

static uint64_t sem_count = 0;

// Mutex used to protect the semaphores pool
static
pthread_mutex_t sem_pool_lock = PTHREAD_MUTEX_INITIALIZER;


#define yotta_sem_all_used(sem_s) \
    ((sem_s)->used & (~(0ull)))

#define yotta_sem_used(sem_s, idx) \
    ((sem_s)->used & (1 << (idx)))


uint64_t
yotta_sem_fetch(sem_t ** out_sem)
{
    yotta_assert(out_sem != NULL);

    pthread_mutex_lock(&sem_pool_lock);

    {
        uint8_t sem_found = 0;
        uint64_t chunk_idx = 0;
        yotta_sem_t ** current_chunk = &sem_pool;

        while(!sem_found)
        {
            // If the current chunk is empty, we allocate a new one
            if(*current_chunk == NULL)
            {
                *current_chunk = yotta_alloc_s(yotta_sem_t);
                memset(*current_chunk, 0, sizeof(yotta_sem_t));
            }

            if(!yotta_sem_all_used(*current_chunk))
            {
                // We search for the first free semaphore
                for(int i = 0; i < YOTTA_SEM_CHUNK_SIZE; i++)
                {
                    // Skip if the semaphore is currently in use
                    if(yotta_sem_used(*current_chunk, i))
                    {
                        continue;
                    }

                    sem_t * free_sem = &(*current_chunk)->sem[i];

                    // If the free semphore has not been initialized, ...
                    if((chunk_idx * YOTTA_SEM_CHUNK_SIZE + i) >= sem_count)
                    {
                        // ... we initialize it
                        if(sem_init(free_sem, 0, 0) == -1)
                        {
                            yotta_logger_error("sem_init failed\n");
                            pthread_mutex_unlock(&sem_pool_lock);
                            return YOTTA_UNEXPECTED_FAIL;
                        }
                        sem_count++;
                    }


                    (*current_chunk)->used |= i;
                    *out_sem = free_sem;
                    sem_found = 1;
                    break;
                }
            }

            current_chunk = &(*current_chunk)->next;
            chunk_idx++;
        }
    }

    pthread_mutex_unlock(&sem_pool_lock);

    return YOTTA_SUCCESS;
}

void
yotta_sem_release(sem_t * sem)
{
    yotta_assert(sem != NULL);
    yotta_assert(sem_pool != NULL);

    pthread_mutex_lock(&sem_pool_lock);

    {
        yotta_sem_t * current_chunk = sem_pool;

        while(current_chunk != NULL)
        {
            // We try to find the semaphore in the current chunk
            for(int i = 0; i < YOTTA_SEM_CHUNK_SIZE; i++)
            {
                if(sem == &current_chunk->sem[i])
                {
                    current_chunk->used &= ~(1 << i);
                    pthread_mutex_unlock(&sem_pool_lock);
                    return;
                }
            }

            current_chunk = current_chunk->next;
        }
    }

    pthread_mutex_unlock(&sem_pool_lock);

    yotta_crash_msg("Unknown semaphore: %p", (void *) sem);
}
