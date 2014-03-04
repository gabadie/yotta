
//#include "testhelper_memory.h"
#include "testhelper_init.h"
#include "testhelper_lorem.h"


/*
 * Tests a group barrier on dispatched threads
 */
static
void
test_context_basics()
{
    yotta_context_t context;

    testhelper_lorem(&context, sizeof(context));

    test_assert(yotta_context_init(&context, 0) == 0);
    test_assert(yotta_context_destroy(&context) == 0);
}


int
main()
{
    testhelper_init();
    //testhelper_memory_setup();

    test_context_basics();

    return 0;
}
