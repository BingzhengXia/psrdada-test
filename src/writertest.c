#include "ipcio.h"
#include "multilog.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void usage()
{
    fprintf(stdout,
            "writer [options]\n"
            " -g gigabytes   number of gigabytes to write\n"
            " -v             verbose mode\n");
}

int main(int argc, char **argv)
{
    ipcio_t ipc;
    multilog_t *log;
    uint64_t bytes_to_write = 0;
    uint64_t data_size = 1024 * 1024;
    char *data = NULL;
    char verbose = 0;
    float gigabytes = 0.0;
    const float one_gigabyte = 1024.0 * 1024.0;
    key_t key = 0x70;

    int arg = 0;
    while ((arg = getopt(argc, argv, "g:v")) != -1) {
        switch (arg) {
        case 'g':
            gigabytes = atof(optarg);
            bytes_to_write = (uint64_t)(gigabytes * one_gigabyte);
            break;
        case 'v':
            verbose = 1;
            break;
        default:
            usage();
            return 0;
        }
    }

    if (bytes_to_write == 0) {
        fprintf(stderr, "Error: You must specify a non-zero number of gigabytes to write using the -g option.\n");
        return EXIT_FAILURE;
    }

    log = multilog_open("writer", 0);
    multilog_add(log, stderr);

    // 创建并连接到共享内存
    if (ipcio_create(&ipc, 0x70, 64, data_size, 1) < 0) {
        multilog(log, LOG_ERR, "Failed to create shared memory buffer\n");
        return EXIT_FAILURE;
    }

    if (ipcio_open(&ipc, 'w') < 0) {
        multilog(log, LOG_ERR, "Failed to open shared memory buffer for writing\n");
        return EXIT_FAILURE;
    }

    data = (char *)malloc(data_size);
    assert(data != NULL);

    if (verbose) {
        fprintf(stderr, "Writing %" PRIu64 " bytes to data block\n", bytes_to_write);
    }

    while (bytes_to_write > 0) {
        if (data_size > bytes_to_write)
            data_size = bytes_to_write;

        if (ipcio_write(&ipc, data, data_size) < 0) {
            multilog(log, LOG_ERR, "Could not write %" PRIu64 " bytes to data block\n", data_size);
            return EXIT_FAILURE;
        }
        else{
            multilog(log, LOG_INFO, "Wrote %" PRIu64 " bytes to data block\n", bytes_to_write);
        }

        bytes_to_write -= data_size;
    }

    ipcio_close(&ipc);
    ipcio_disconnect(&ipc);
    //ipcio_destroy(&ipc);
    free(data);

    return EXIT_SUCCESS;
}

