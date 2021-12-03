#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[512];

void ping_pong(int exchange_num)
{
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    int pid1, pid2;
    if ((pid1 = fork()) == 0)
    {
        // currently in first child process
        int ping_count = 0;
        int start_time = uptime();
        close(p2[0]); // not reading on second pipe
        close(p1[1]); // not writing on first pipe
        while (ping_count < exchange_num)
        {
            write(p2[1], "ping", 5);
            read(p1[0], buf, 5);
            ping_count++;
            // printf("ping_count = %d\n", ping_count); // print to stdout the ping_count
        }
        printf("%d ping pongs finished\n", exchange_num);
        printf("used %d ms\n", (uptime() - start_time) * 100); /* each clock tick is 1/10 second */
        // finished everything, exiting
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else
    {
        // still in parent process
        if ((pid2 = fork()) == 0)
        {
            // in second child process
            int ping_count = 0;
            close(p1[0]); // not writing on second pipe
            close(p2[1]); // not reading on first pipe
            while (ping_count < exchange_num)
            {
                write(p1[1], "ping", 5);
                read(p2[0], buf, 5);
                ping_count++;
            }
            close(p1[1]);
            close(p2[0]);
            exit(0);
        }
        else
        {
            // in parent process
            wait(0);
            wait(0);
            close(p1[0]);
            close(p1[1]);
            close(p2[0]);
            close(p2[1]);
            printf("Finished Process, Gracefully Exiting\n");
            exit(0);
        }
    }
    printf("Should not run this\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    /* create a pipe, fork two processes to ping pong to each other */
    int exchange_num = 100000; // default to ping pong 1000 times
    ping_pong(exchange_num);
    exit(0);
}
