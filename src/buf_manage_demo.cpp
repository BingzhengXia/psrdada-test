#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <values.h>
#include <sys/types.h>
#include<iostream>
#include "ipcbuf.h" 
#include "ipcbuf_manage.h"
int main(int argc, char ** argv){
    key_t key = 0x69;
    key_t key2 = 0x70;
    uint64_t nbufs = 100;
    uint64_t bufsz = 1024*1024;
    unsigned int nreaders = 2;
    uint64_t bytesio = 0;
    char *bufw ;
    char *bufr;
    char *bufr2;

    Buffer_Manager bm(key, nbufs, bufsz, nreaders);
    bm.buffer_init();
    bm.get_buf_info();
    bufw = bm.get_buf_write();
    strcpy(bufw, "transfer data test!");
    // bufw = "MinMin baobao!";
    bm.mark_write_done();
    bufr = bm.get_buf_read(&bytesio);
    printf("bytesnum: %lu kb\n", bytesio/1024);
    bm.mark_read_done();
    printf("key = 69: %s\n", bufr);

    Buffer_Manager bm2(key, true);
    bm2.buffer_connect();
    bufr2 = bm2.get_buf_read(&bytesio);
    bm2.mark_read_done();
    printf("key = 69 connect_mode: %s\n", bufr2);

    Buffer_Manager bm3(key2, nbufs, bufsz, nreaders);
    bm3.buffer_init();
    bufw = bm3.get_buf_write();
    strcpy(bufw, bufr2);
    bm3.mark_write_done();
    bufr = bm3.get_buf_read(&bytesio);
    bm3.mark_read_done();
    printf("key = 70: %s\n", bufr);
    
    return 0;


}