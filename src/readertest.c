#include "ipcio.h"
#include "multilog.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void usage()
{
    fprintf(stdout,
            "reader [options]\n"
            " -v             verbose mode\n");
}

int main(int argc, char **argv)
{
    ipcio_t ipc;
    multilog_t *log;
    char verbose = 0;
    uint64_t data_size = 1024 * 1024;
    char *data = NULL;
    key_t key = 0x70;

    int arg = 0;
    while ((arg = getopt(argc, argv, "v")) != -1) {
        switch (arg) {
        case 'v':
            verbose = 1;
            break;
        default:
            usage();
            return 0;
        }
    }

    log = multilog_open("reader", 0);
    multilog_add(log, stderr);

    // 连接到共享内存缓冲区
    if (ipcio_connect(&ipc, key) < 0) {
        multilog(log, LOG_ERR, "Failed to connect to shared memory buffer\n");
        return EXIT_FAILURE;
    }
    else{
        multilog(log, LOG_ERR, "Success connect to shared memory buffer\n");
        

    }

    if (ipcio_open(&ipc, 'r') < 0) {
        multilog(log, LOG_ERR, "Failed to open shared memory buffer for reading\n");
        return EXIT_FAILURE;
    }
    else{
        multilog(log, LOG_ERR, "Success open shared memory buffer for reading\n");
    }

    data = (char *)malloc(data_size);
    assert(data != NULL);

    uint64_t bytes_read = 0;
    while ((bytes_read = ipcio_read(&ipc, data, data_size)) > 0) {
        //fprintf(stderr, "Read %" PRIu64 " bytes: %s\n", bytes_read, data);
        multilog(log, LOG_ERR, "Read %" PRIu64 " bytes: %s\n", bytes_read, data);
        if (verbose) {
            fprintf(stderr, "Read %" PRIu64 " bytes: %s\n", bytes_read, data);
        }
    }

    ipcio_close(&ipc);
    ipcio_disconnect(&ipc);
    free(data);

    return EXIT_SUCCESS;
}

