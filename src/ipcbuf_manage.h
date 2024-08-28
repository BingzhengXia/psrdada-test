#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <iostream>
// #include "ipcbuf.h"

class Buffer_Manager
{
private:
    key_t key;
    uint64_t nbufs;
    uint64_t bufsz;
    unsigned int nreaders;
    ipcbuf_t ringbuf = IPCBUF_INIT;
    uint64_t count;
    uint64_t blockbytes;
    int gpu_device_id;
    bool is_connect_mode = false; // 用来选择自行创建缓冲区还是连接到已经存在的缓冲区

public:
    // 构造函数  第一个构造函数负责创建缓冲区，第二个构造函数用于连接到已有缓冲区时接受key值
    Buffer_Manager(key_t key_, uint64_t nbufs_, uint64_t bufsz_, unsigned int nreaders_, int gpu_device_id_);
    Buffer_Manager(key_t key_);

    // 析构函数
    ~Buffer_Manager();

    // 根据参数创建并连接缓冲区
    void buffer_init();

    // 对缓冲区加写锁和解写锁
    void lock_buf_write();
    void unlock_buf_write();

    // 获取用于写入的缓冲区buffer，大小等于bufsz
    char *get_buf_write();

    // 标记buffer写入完成
    void mark_write_done();

    // 连接到已存在缓冲区，采用第二个构造函数时需要连接buffer
    void buffer_connect();

    // 断开连接
    void buffer_disconnect();

    // 对缓冲区加读锁和解读锁
    void lock_buf_read();
    void unlock_buf_read();

    // 获取用于读取的缓冲区buffer，大小等于bytes
    char *get_buf_read(uint64_t *bytes);

    // 标记buffer读取完成
    void mark_read_done();

    // 获取共享内存键
    key_t get_key();

    // 获取缓冲区信息
    void print_buf_info();
};

#endif // BUFFER_MANAGER_H