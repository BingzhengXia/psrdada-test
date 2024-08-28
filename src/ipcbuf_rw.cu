#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <values.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "ipcbuf.h"

int main(int argc, char** argv) {
    key_t key = 0x69;
    int arg;
    uint64_t nbufs = 64; 
    uint64_t bufsz = 1024*1024; 

    uint64_t total_data_mb = 100; 
    uint64_t total_data_bytes = total_data_mb * 1024 * 1024;

    uint64_t* buf = NULL;
    uint64_t count = 0;
    uint64_t bytesio = 0;

    ipcbuf_t ringbuf = IPCBUF_INIT;
    unsigned int np = 4;
    pid_t pid;
    

    int debug = 0;
    while ((arg = getopt(argc, argv, "b:dhn:k:v:t:")) != -1) {
        switch (arg) {
            case 'h':
                fprintf(stderr,
                "test_ipcbuf [options]\n"
                " -b block_size  Set the size of each block in ring buffer \n"
                " -n nblock      Set the number of blocks in ring buffer \n"
                " -d             Debug mode \n"
                " -k key         Key to shared memory \n"

                " -t total_data  Total data to write in MB\n");
                return 0;

            case 'b':
                if (sscanf(optarg, "%" PRIu64 "", &bufsz) < 1) {
                    fprintf(stderr, "test_ipcbuf could not parse -b %s", optarg);
                    return -1;
                }
                break;

            case 'd':
                debug = 1;
                break;

            case 'n':
                if (sscanf(optarg, "%" PRIu64 "", &nbufs) < 1) {
                    fprintf(stderr, "test_ipcbuf could not parse -n %s", optarg);
                    return -1;
                }
                break;

            case 'k':
                key = atoi(optarg);
                break;

            case 't':
                if (sscanf(optarg, "%" PRIu64 "", &total_data_mb) < 1) {
                    fprintf(stderr, "test_ipcbuf could not parse -t %s", optarg);
                    return -1;
                }
                total_data_bytes = total_data_mb * 1024 * 1024;
                break;
        }
    }
    // Writer process
    if (ipcbuf_create_work(&ringbuf, key, nbufs, bufsz, np, 1) < 0) {
        fprintf(stderr, "Error creating shared memory ring buffer\n");
        return -1;
    }
        // for (int i =  0; i < np; i++) 
        //     pid = fork();
      for (int i =  0; i < np; i++) {
           pid = fork();
            if (pid == 0) {
        // 子进程立即退出循环，防止子进程再创建子进程
             break;
           } else if (pid < 0) {
          fprintf(stderr, "Fork failed\n");
          exit(1);
           }
          }

    if (pid) {


        if (ipcbuf_lock_write(&ringbuf) < 0) {
            fprintf(stderr, "Error ipcbuf_lock_write\n");
            return -1;
        }

        struct timeval start, end;
        double total_time;

        gettimeofday(&start, NULL); //计时

        // while (count < total_data_bytes) {
        while (1) {
            buf = (uint64_t*)ipcbuf_get_next_write(&ringbuf);
            if (!buf) {
                fprintf(stderr, "error ipcbuf_get_next_write\n");
                return -1;
            }

            //填写数据
            for (uint64_t i64 = 0; i64 < bufsz / sizeof(uint64_t); i64++) {
                buf[i64] = count;
                count += sizeof(uint64_t);
            }

            if (ipcbuf_mark_filled(&ringbuf, bufsz) < 0) {
                fprintf(stderr, "error ipcbuf_mark_filled\n");
                return -1;
            }
        }
        if (ipcbuf_unlock_write(&ringbuf) < 0) {
            fprintf(stderr, "error ipcbuf_unlock_write\n");
            return -1;
        }

        gettimeofday(&end, NULL); // Stop
        total_time = (end.tv_sec - start.tv_sec) * 1000.0;
        total_time += (end.tv_usec - start.tv_usec) / 1000.0;

        fprintf(stderr, "Writing completed in %.2f ms\n", total_time);



        if (debug) {
            fprintf(stderr, "Pause before destroy\n");
        }
        //  for (int i = 0; i < np; i++)
          wait(NULL);
          printf("All child processes have finished.\n");
        ipcbuf_destroy(&ringbuf);
        
    } else {
    // if(pid == 0){
        // Reader process
        sleep(1);

        if (ipcbuf_connect(&ringbuf, key) < 0) {
            fprintf(stderr, "Error connecting to shared memory ring buffer\n");
            return -1;
        }
        nbufs = ipcbuf_get_nbufs(&ringbuf);
        bufsz = ipcbuf_get_bufsz(&ringbuf);

        struct timeval start, end;
        double total_time;

        gettimeofday(&start, NULL); 

        

        // while (count < total_data_bytes) {
        // while (1) {
        for(int i = 0; i<nbufs; i++){
          if (ipcbuf_lock_read(&ringbuf) < 0) {
            fprintf(stderr, "Error ipcbuf_lock_read\n");
            return -1;
        }
            buf = (uint64_t*)ipcbuf_get_next_read(&ringbuf, &bytesio);
            if (!buf) {
                fprintf(stderr, "Error ipcbuf_get_next_read\n");
                return -1;
            }

            fprintf(stderr, "Read %" PRIu64 " bytes successfully by %d\n", bytesio,getpid());

            if (ipcbuf_mark_cleared(&ringbuf) < 0) {
                fprintf(stderr, "error ipcbuf_mark_cleared\n");
                return -1;
            }
             if(ipcbuf_unlock_read(&ringbuf) < 0){
                fprintf(stderr, "error ipcbuf_unlock_read\n");
                return -1;
        }
            
        }

        gettimeofday(&end, NULL); // Stop timer
        total_time = (end.tv_sec - start.tv_sec) * 1000.0;
        total_time += (end.tv_usec - start.tv_usec) / 1000.0;
        // if(ipcbuf_unlock_read(&ringbuf) < 0){
        //   fprintf(stderr, "error ipcbuf_unlock_read\n");
        //   return -1;
        // }

        fprintf(stderr, "Reading completed in %.2f ms\n", total_time);

        // if (ipcbuf_reset(&ringbuf) < 0) {
        //     fprintf(stderr, "error ipcbuf_reset\n");
        //     return -1;
        // }
    }
  
  
   
    return 0;
}