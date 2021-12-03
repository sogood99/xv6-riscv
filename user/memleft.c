#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int
main(int argc, char *argv[])
{
    // simply just printf the result of memleft
    printf("Memory Left: %d bytes\n", memleft());
    exit(0);
}
