
#include <string.h>

#include "yotta_semaphore_pool.private.h"
#include "../core/yotta_debug.h"
#include "../core/yotta_logger.private.h"
#include "../core/yotta_memory.h"
#include "../core/yotta_return.h"
#include "../threading/yotta_mutex.h"


typedef struct
yotta_semaphore_deck_s
yotta_semaphore_deck_t;

/*
 * @infos Chunk of a deque of semaphores
 */
struct
yotta_semaphore_deck_s
{
    // List of semaphores
    yotta_semaphore_t sem[sizeof(uint64_t) * 8];

    // Bitfield used to check if a semaphore is in use
    uint64_t used;

    // Pointer to the next semaphores chunk
    yotta_semaphore_deck_t * next;
};

/*
 * Semaphores pool used for thread synchronisation
 */
static
yotta_semaphore_deck_t * sem_pool = NULL;

// Number of semaphores initialized
static
uint64_t sem_count = 0;

/*
 * Mutex used to protect the semaphores pool
 */
static
yotta_mutex_t sem_pool_lock = YOTTA_MUTEX_INITIALIZER;


#define yotta_sem_all_used(sem_s) \
    ((sem_s)->used == ~(0ull))

#define yotta_sem_used(sem_s, idx) \
    ((sem_s)->used & (1ull << (idx)))

#define yotta_sem_all_free(sem_s) \
    ((sem_s)->used == 0ull)


yotta_return_t
yotta_sem_fetch(yotta_semaphore_t ** out_sem)
{
    yotta_assert(out_sem != NULL);

    yotta_mutex_lock(&sem_pool_lock);

    {
        uint8_t sem_found = 0;
        uint64_t chunk_idx = 0;
        yotta_semaphore_deck_t ** current_chunk = &sem_pool;

        while (!sem_found)
        {
            // If the current chunk is empty, we allocate a new one
            if (*current_chunk == NULL)
            {
                *current_chunk = yotta_alloc_s(yotta_semaphore_deck_t);

                memset(*current_chunk, 0, sizeof(yotta_semaphore_deck_t));
            }

            // If the current chunk is totally used, we skip it
            if (yotta_sem_all_used(*current_chunk))
            {
                current_chunk = &(*current_chunk)->next;
                chunk_idx++;
                continue;
            }

            // We search for the first free semaphore
            for (uint64_t i = 0; i < sizeof(uint64_t) * 8; i++)
            {
                // Skip if the semaphore is currently in use
                if (yotta_sem_used(*current_chunk, i))
                {
                    continue;
                }

                yotta_semaphore_t * free_sem = (*current_chunk)->sem + i;

                // If the free semphore has not been initialized, ...
                if ((chunk_idx * sizeof(uint64_t) * 8 + i) >= sem_count)
                {
                    // ... we initialize it
                    if (yotta_semaphore_init(free_sem, 0) != 0)
                    {
                        yotta_logger_error("sem_init failed\n");

                        yotta_mutex_unlock(&sem_pool_lock);

                        return YOTTA_UNEXPECTED_FAIL;
                    }

                    sem_count++;
                }

                (*current_chunk)->used |= (1ull << i);
                *out_sem = free_sem;
                sem_found = 1;
                break;
            }

            current_chunk = &(*current_chunk)->next;
            chunk_idx++;
        }
    }

    yotta_mutex_unlock(&sem_pool_lock);

    return YOTTA_SUCCESS;
}

void
yotta_sem_release(yotta_semaphore_t * sem)
{
    yotta_assert(sem != NULL);
    yotta_assert(sem_pool != NULL);

#if 0
    // @Thierry: what is that for ????
    int waiting_threads = 42;
    sem_getvalue(sem, &waiting_threads);
    yotta_assert(waiting_threads == 0);
#endif

    yotta_mutex_lock(&sem_pool_lock);

    {
        yotta_semaphore_deck_t * current_chunk = sem_pool;

        while (current_chunk != NULL)
        {
            // We try to find the semaphore in the current chunk
            if (sem >= &current_chunk->sem[0] &&
                sem <  &current_chunk->sem[0] + sizeof(uint64_t) * 8)
            {
                current_chunk->used &= ~(1ull << (uint64_t)(sem - &current_chunk->sem[0]));
                yotta_mutex_unlock(&sem_pool_lock);
                return;
            }

            current_chunk = current_chunk->next;
        }
    }

    yotta_mutex_unlock(&sem_pool_lock);

    yotta_crash_msg("Unknown semaphore: %p", (void *) sem);
}

uint64_t
yotta_sem_pool_flush()
{
    yotta_mutex_lock(&sem_pool_lock);

    yotta_semaphore_deck_t * deck = sem_pool;

    sem_pool = NULL;
    yotta_semaphore_deck_t * last_non_free = NULL;

    while (deck != NULL)
    {
        yotta_semaphore_deck_t * tmp = deck;
        deck = deck->next;

        // If all the semaphore of the deck are free, we destroy it..
        if(yotta_sem_all_free(tmp))
        {
            uint64_t nb_sem = (deck == NULL && (sem_count % 64)) ? (sem_count % 64) : 64;
            for(uint64_t i = 0; i < nb_sem; i++)
            {
                yotta_semaphore_destroy(&tmp->sem[i]);
            }
            yotta_free(tmp);
            sem_count -= nb_sem;
            continue;
        }

        // .. Otherwise we rebuild a new semaphore pool with the
        // remaining decks
        if(last_non_free != NULL)
        {
            last_non_free->next = tmp;
            last_non_free = last_non_free->next;
        }
        else
        {
            sem_pool = tmp;
            last_non_free = tmp;
        }
    }

    yotta_mutex_unlock(&sem_pool_lock);

    return sem_count;
}
