#include <stdio.h>
// #include <stdlib.h>
#include <unistd.h>
#include <string.h>
// #include <values.h>
#include <sys/types.h>
#include <iostream>
#include "ipcbuf.h"
#include "ipcbuf_manage.h"
#include <sys/wait.h>
using namespace std;
Buffer_Manager::Buffer_Manager(key_t key_ , uint64_t nbufs_ , uint64_t bufsz_ , unsigned int nreaders_, int gpu_device_id_ = -1)
    : key(key_), nbufs(nbufs_), bufsz(bufsz_), nreaders(nreaders_),gpu_device_id(gpu_device_id_)
{
}
Buffer_Manager::Buffer_Manager(key_t key_) : key(key_)
{
    is_connect_mode = true;

}
Buffer_Manager::~Buffer_Manager()
{
    if (is_connect_mode)
    {
    }
    else
    {
        if (ipcbuf_destroy(&ringbuf) < 0)
            perror("ipcbuf_destroy error");
    }
}
void Buffer_Manager::buffer_init()
{
    if (ipcbuf_create_work(&ringbuf, key, nbufs, bufsz, nreaders,gpu_device_id) < 0)
        perror("ipcbuf_create error");
    return;
}
void Buffer_Manager::lock_buf_write()
{
    if (ipcbuf_lock_write(&ringbuf) < 0)
    {
        perror("ipcbuf_lock_write error");
        return;
    }
}

char *Buffer_Manager::get_buf_write()
{
    char *buf;
    if ((buf = ipcbuf_get_next_write(&ringbuf)) == NULL)
    {
        perror("ipcbuf_get_next_write error");
        return nullptr;
    }
    return buf;
}
void Buffer_Manager::mark_write_done()
{
    if (ipcbuf_mark_filled(&ringbuf, bufsz) < 0)
    {
        perror("ipcbuf_mark_filled error");
        return ;
    }

}

void Buffer_Manager::unlock_buf_write()
{

    if (ipcbuf_unlock_write(&ringbuf) < 0)
    {
        perror("ipcbuf_unlock_write error");
        return;
    }
}
void Buffer_Manager::buffer_connect()
{
    if (ipcbuf_connect(&ringbuf, key) < 0)
    {
        perror("ipcbuf_connect error");
        return;
    }
}
void Buffer_Manager::buffer_disconnect()
{
    if (ipcbuf_disconnect(&ringbuf) < 0)
    {
        perror("ipcbuf_disconnect error");
        return;
    }
}
void Buffer_Manager::lock_buf_read()
{
    if (ipcbuf_lock_read(&ringbuf) < 0)
    {
        perror("ipcbuf_lock_read error");
        return;
    }
}
char *Buffer_Manager::get_buf_read(uint64_t *bytes)
{
    char *buf;
    if ((buf = ipcbuf_get_next_read(&ringbuf, bytes)) == NULL)
    {
        perror("ipcbuf_get_next_read error");
        return nullptr;
    }
    return buf;
}
void Buffer_Manager::mark_read_done()
{
    if (ipcbuf_mark_cleared(&ringbuf) < 0)
    {
        perror("ipcbuf_mark_cleared error");
        return ;
    }

}
void Buffer_Manager::unlock_buf_read()
{

    if (ipcbuf_unlock_read(&ringbuf) < 0)
    {
        perror("ipcbuf_unlock_read error");
        return;
    }
}

key_t Buffer_Manager::get_key()
{
    return *ringbuf.shmkey;
}
void Buffer_Manager::print_buf_info()
{
    cout << "nbufs:\t" << ipcbuf_get_nbufs(&ringbuf) << endl;
    cout << "bufsz:\t" << ipcbuf_get_bufsz(&ringbuf) / 100024 << "kb" << endl;
    cout << "nreaders:\t" << ipcbuf_get_nreaders(&ringbuf) << endl;
    cout << "key:\t" << *(ringbuf.shmkey) << hex << *(ringbuf.shmkey) << endl;
}
int main(int argc, char **argv)
{
    key_t key = 0x69;
    key_t key2 = 0x70;
    uint64_t nbufs = 64;
    uint64_t bufsz = 1024 * 1024;
    unsigned int nreaders = 1;
    uint64_t bytesio = 0;
    int test_nums = 1000;
    char *buf1;
    char *buf2;
    char *buf3;
    if (fork())
    {
        Buffer_Manager bm1(key, nbufs, bufsz, nreaders);
        bm1.buffer_init();
        bm1.print_buf_info();
        bm1.lock_buf_write();
        for (int i = 0; i < test_nums; i++)
        {
            buf1 = bm1.get_buf_write();
            string str = "transfer data"+to_string(i)+" test!";
            strcpy(buf1, str.c_str());
            bm1.mark_write_done();
            printf("process 1 pid:%d write data: %s\n",getpid(), str.c_str());
        }
        bm1.unlock_buf_write();
        printf("process1 write done!\n");
        wait(NULL);
    }
    else
    {
        if (fork())
        {
            sleep(0.01);
            Buffer_Manager bm2(key);
            bm2.buffer_connect();
            Buffer_Manager bm3(key2);
            bm3.buffer_connect();
            bm2.lock_buf_read();
            bm3.lock_buf_write();
            for (int i = 0; i < test_nums; i++)
            {
                buf2 = bm2.get_buf_read(&bytesio);
                bm2.mark_read_done();
                printf("process 2 pid:%d read data: %s ----read No.%d\n", getpid(), buf2, i);
                strcpy(bm3.get_buf_write(), buf2);
                bm3.mark_write_done();
              
            }
            bm2.unlock_buf_read();
            bm3.unlock_buf_write();
            printf("process2 read and write done!\n");
            wait(NULL);
        }
        else
        {
            Buffer_Manager bm3(key2, nbufs, bufsz, nreaders);
            bm3.buffer_init();
            bm3.lock_buf_read();
            for (int i = 0; i < test_nums; i++)
            {
                buf3 = bm3.get_buf_read(&bytesio);
                bm3.mark_read_done();
                printf("process 3 pid:%d read data: %s ----read No.%d\n",getpid(), buf3, i);
            }
            bm3.unlock_buf_read();
            printf("process3 read done!\n");
            // printf("key = 70: %s\n", buf3);
        }
    }

    return 0;
}