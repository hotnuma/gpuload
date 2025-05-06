
#include "cpuload.h"
#include "gpuload.h"
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define APPNAME "gpuload"

#if 0
static void error_exit(const char *msg)
{
    if (!msg || *msg == '\0')
    {
        msg = "an error occurred";
    }

    printf("*** %s\nabort...\n", msg);

    exit(EXIT_FAILURE);
}
#endif

static void usage_exit()
{
    printf("*** usage :\n");
    printf("%s\n", APPNAME);
    printf("%s -gt 20\n", APPNAME);
    printf("abort...\n");

    exit(EXIT_FAILURE);
}

int msleep(long msec)
{
    // https://stackoverflow.com/questions/1157209/

    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    setbuf(stdout, NULL);

    int n = 1;

    int opt_gputrigger = -1;

    while (n < argc)
    {
        // gpu trigger
        if (strcmp(argv[n], "-gt") == 0)
        {
            if (++n >= argc)
                usage_exit();

            opt_gputrigger = atoi(argv[n]);
        }

        else
        {
            usage_exit();
        }

        ++n;
    }

    GpuData gpudata = {0};
    gpudata.show_percentage = true;

    cpuload();
    gpuload(&gpudata);

    while (1)
    {
        msleep(1000);

        double cpu = cpuload();
        float gpu = gpuload(&gpudata);

        if (opt_gputrigger > -1 && gpu > opt_gputrigger)
            opt_gputrigger = 0;

        if (gpu >= opt_gputrigger)
            printf("%f\t%f\n", cpu, gpu);
    }

    return EXIT_SUCCESS;
}


