/**
 * 爬虫epoll类
 * 作者：张春阳
 * 创建时间：2014-11-14
 */
#pragma once
#ifndef _CRAWLER_EPOLL_H
#define _CRAWLER_EPOLL_H

#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <iostream>
//回调中的响应类型
const int EV_READ = 0x01;
const int EV_WRITE = 0x02;
const int EV_DROP = 0x04;
const int EV_ACCEPT = 0x08;

class crawler_epoll
{
public:
    //1为ET模式，0为LT模式
    crawler_epoll(const int type = 1);
    //析构函数
    ~crawler_epoll();
    //epoll初始化，设置回调函数
    int epoll_init(void*(*)(const int,const int));
    //设置成server模式
    int epoll_set_server(const int);
    //向epoll中添加fd和注册的事件
    int epoll_add(const int,const uint32_t);
    //移除epoll中的fd和相应的事件
    int epoll_remove(const int);
    //修改epoll中fd的注册事件
    int epoll_modify(const int,const uint32_t);
    //等待事件触发
    int epoll_run(const int);
private:
    int server_fd;
    int epo_type;
    struct epoll_event *events;
    int epo_fd;
    bool epoll_manage(const int,const uint32_t,const int);
    void* (*epoll_event_callback)(const int,const int);
};

#endif
