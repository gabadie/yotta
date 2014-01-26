
#include <yotta.h>
#include <mk_test.h>


int
main(int argc, char ** argv)
{
    test_assert("hello world", 1);

    yotta_init(argc, argv);

    return 0;
}

