#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

#include "ipcio.h"
#include "ipcbuf.h"

int main (int argc, char** argv)
{
    key_t key = 0x11;
    int arg;

    // 默认缓冲区块数和每块大小
    uint64_t nbufs = 64;
    uint64_t bufsz = 1 * 1024 * 1024; // 1MB

    uint64_t total_mb = 1024; // 写入总数据量，单位为MB
    uint64_t total_bytes = total_mb * 1024 * 1024; // 总数据量，单位为字节

    uint64_t count = 0;
    ipcio_t ringbuf = IPCIO_INIT;
    uint64_t bytesio = 0;

    while ((arg = getopt(argc, argv, "b:n:k:t:")) != -1) {
        switch (arg) {
        case 'b':
            if (sscanf(optarg, "%"PRIu64"", &bufsz) < 1) {
                fprintf(stderr, "Could not parse -b %s", optarg);
                return -1;
            }
            break;

        case 'n':
            if (sscanf(optarg, "%"PRIu64"", &nbufs) < 1) {
                fprintf(stderr, "Could not parse -n %s", optarg);
                return -1;
            }
            break;

        case 'k':
            key = atoi(optarg);
            break;

        case 't':
            if (sscanf(optarg, "%"PRIu64"", &total_mb) < 1) {
                fprintf(stderr, "Could not parse -t %s", optarg);
                return -1;
            }
            total_bytes = total_mb * 1024 * 1024;
            break;

        }
    }

    if (fork()) {
        // 父进程 - 写入数据
        fprintf(stderr, "Creating shared memory ring buffer."
                " nbufs=%"PRIu64" bufsz=%"PRIu64"\n", nbufs, bufsz);

        if (ipcio_create(&ringbuf, key, nbufs, bufsz, 1) < 0) {
            fprintf(stderr, "Error creating shared memory ring buffer\n");
            return -1;
        }

        ipcio_open(&ringbuf, 'w');

        uint64_t* buffer = malloc(bufsz);
        assert(buffer != 0);
        // if (ipcio_start (&ringbuf, 0) < 0)
	    // return -1;

        for (uint64_t i = 0; i < total_bytes; i += bufsz) {
            // 填充数据
            for (uint64_t j = 0; j < bufsz / sizeof(uint64_t); j++) {
                buffer[j] = count++;
            }
            //ipcio_start (&ringbuf, 0);

            bytesio = ipcio_write(&ringbuf, (char*)buffer, bufsz);
            //ipcio_stop(&ringbuf);
            if (bytesio != bufsz) {
                fprintf(stderr, "Error writing to ring buffer\n");
                return -1;
            }
        }
        // if(ipcio_stop(&ringbuf))
        // return -1;

        fprintf(stderr, "Finished writing %lu MB\n", total_mb);
        
        //ipcio_stop(&ringbuf);
        
        //ipcio_close(&ringbuf);
        wait(0); // 等待子进程
        ipcio_destroy(&ringbuf);
        free(buffer);
        
        
        
    } else {
        // 子进程 - 读取数据
        sleep(1); 
        fprintf(stderr, "Connecting to shared memory ring buffer\n");
        if (ipcio_connect(&ringbuf, key) < 0) {
            fprintf(stderr, "Error connecting to shared memory ring buffer\n");
            return -1;
        }

        if(ipcio_open(&ringbuf, 'R')<0){
            fprintf(stderr, "Error open buffer to read\n");
            return -1;

        }

        // uint64_t* buffer = malloc(bufsz);
        // assert(buffer != 0);
         fprintf(stderr, "Success connect and open the shared memory ring buffer\n");

        // while ((int)(bytesio = ipcio_read(&ringbuf, (char*)buffer, bufsz)) > 0) {
        //     fprintf(stderr, "Read %"PRIu64" bytes\n", bytesio);
        // }
        //buffer = ipcio_open_block_read(&ringbuf, bufsz);
        char *buf_read_p = NULL;
        uint64_t block_id ;
        uint64_t block_size;
        buf_read_p = ipcio_open_block_read(&ringbuf, &block_size, &block_id);
        fprintf(stderr, "block_id = %"PRIu64"\n", block_id);
        fprintf(stderr, "buf_read_p = %s\n", buf_read_p);
        fprintf(stderr, "block_size = %"PRIu64"\n", block_size);
        if( ipcio_close_block_read(&ringbuf, block_size)<0)
        fprintf(stderr, "Error close block read\n");
        

       

        ipcio_close(&ringbuf);
        // free(buffer);
    }

    return 0;
}