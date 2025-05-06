
#include "cpuload.h"
#include "gpuload.h"
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define APPNAME "GpuData"

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

static void usage_exit()
{
    printf("*** usage :");
    printf("%s -m -s <projectname>", APPNAME);
    printf("%s <projectname>", APPNAME);
    printf("abort...\n");

    exit(EXIT_FAILURE);
}
#endif

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

    GpuData gpudata = {0};
    gpudata.show_percentage = true;

    cpuload();
    gpuload(&gpudata);

    while (1)
    {
        msleep(1000);

        double cpu = cpuload();
        float gpu = gpuload(&gpudata);

        printf("%f\t%f\n", cpu, gpu);
    }

    return EXIT_SUCCESS;
}


