/**
 * 定时器类,一个实例为一个定时器
 * 作者：张春阳
 * 创建时间：2014-11-5
 */
#ifndef _CRAWLER_TIME_ALARM_H
#define _CRAWLER_TIME_ALARM_H

#include <pthread.h>

class crawler_time_alarm
{
public:
    //无参数构造函数
    crawler_time_alarm();
    //有参数构造函数
    crawler_time_alarm(long,long,void (*)(void*),void*,int repeat_time = 1);
    //析构函数
    ~crawler_time_alarm();
    //创建定时器
    int time_alarm_create();
    //设置定时器
    int time_alarm_set(long,long,void (*)(void*),void*,int repeat_time = 1);
    //获取定时器剩余的次数
    int time_alarm_get();
    //销毁定时器
    int time_alarm_destroy();
private:
    pthread_t _time_alarm_thread;
    long _alarm_second,_alarm_microsecond;
    int _alarm_counts;
    void *_time_alarm_func_arg;
    void (*_time_alarm_func)(void*);

    void _constructor();
    static void* _thread_func(void *p)
    {
        (static_cast<crawler_time_alarm*>(p))->_time_alarm_thread_func(p);
    }
    void _time_alarm_thread_func(void*);
};

#endif
