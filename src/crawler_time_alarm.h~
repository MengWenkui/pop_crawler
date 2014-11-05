/**
 * 爬虫定时器类，支持同一线程中多个定时器
 * 作者：张春阳
 * 创建时间：2014-11-5
 */
#pragma once
#ifndef _CRAWLER_TIME_ALARM
#define _CRAWLER_TIME_ALARM

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

class crawler_time_alarm
{
public:
    //无参数构造函数
    crawler_time_alarm();
    //有参数构造函数
    crawler_time_alarm(long,long,void* (*)(void*),void*,int repeat_time = 1);
    //析构函数
    ~crawler_time_alarm();
    //创建定时器
    int time_alarm_create();
    //设置定时器
    int time_alarm_set(long,long,void* (*)(void*),void*,int repeat_time = 1);
    //获取定时器剩余的次数
    int time_alarm_get();
    //销毁定时器
    int time_alarm_destroy();
private:
    pthread_t time_alarm_thread;
    long alarm_second,alarm_microsecond;
    int err_code;
    int alarm_counts;
    void* (*time_alarm_func)(void*);
    void *time_alarm_func_arg;
    static void *thread_func(void *p)
    {
        (static_cast<crawler_time_alarm*>(p))->time_alarm_thread_func(p);
    }
    void time_alarm_thread_func(void*);
};

#endif
