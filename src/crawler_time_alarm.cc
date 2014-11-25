#include "crawler_time_alarm.h"
#include "stdafx.h"
#include <sys/time.h>
#include <stdio.h>

void crawler_time_alarm::_constructor()
{
    _alarm_second = 0;
    _alarm_microsecond = 0;
    _time_alarm_func = NULL;
    _time_alarm_func_arg = NULL;
    err_code = 0;
}

crawler_time_alarm::crawler_time_alarm()
{
    _constructor();
}

crawler_time_alarm::crawler_time_alarm(long second,long microsecond,void (*callback)(void*),void *arg,int repeat_time)
{
    _alarm_second = second;
    _alarm_microsecond = microsecond;
    _time_alarm_func = callback;
    _time_alarm_func_arg = arg;
    _alarm_counts = repeat_time;
    err_code = 0;
}

crawler_time_alarm::~crawler_time_alarm()
{
    if(_time_alarm_thread)
    {
        pthread_cancel(_time_alarm_thread);
        pthread_join(_time_alarm_thread,NULL);
    }
}

//创建定时器
int crawler_time_alarm::time_alarm_create()
{
    //创建定时器线程
    if(pthread_create(&_time_alarm_thread,NULL,&_thread_func,this))
    {
        err_code = 900;
        return err_code;
    }
    return 0;
}

//设置定时器
int crawler_time_alarm::time_alarm_set(long second,long microsecond,void (*callback)(void*),void *arg,int repeat_time)
{
    //定时器时间（秒）
    _alarm_second = second;
    //定时器时间（毫秒）
    _alarm_microsecond = microsecond;
    //定时器回调函数
    _time_alarm_func = callback;
    //定时器回调函数参数
    _time_alarm_func_arg = arg;
    //定时器重复数
    _alarm_counts = repeat_time;

    return 0;
}

//销毁定时器
int crawler_time_alarm::time_alarm_destroy()
{
    //给定时器线程发送终止信号
    if(pthread_cancel(_time_alarm_thread))
    {
        err_code = 900;
        return err_code;
    }
    
    //等待线程结束
    if(pthread_join(_time_alarm_thread,NULL))
    {
        err_code = 900;
        return err_code;
    }

    return 0;
}

//获取定时器剩余的次数,负数为一直定时
int crawler_time_alarm::time_alarm_get()
{
    return _alarm_counts;
}

void crawler_time_alarm::_time_alarm_thread_func(void *obj)
{
    struct timeval alarm_timeval;
    crawler_time_alarm *para = (crawler_time_alarm*)obj;

    do
    {
        pthread_testcancel();
        alarm_timeval.tv_sec = _alarm_second;
        alarm_timeval.tv_usec = _alarm_microsecond;
        select(0,NULL,NULL,NULL,&alarm_timeval);
        (*(para->_time_alarm_func))(para->_time_alarm_func_arg);
        para->_alarm_counts--;
    }
    while(para->_alarm_counts);
}
