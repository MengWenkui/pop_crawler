/**
 * 日志记录类
 * 作者：张春阳
 * 创建日期：2014-10-24
 */
#pragma once
#ifndef _CRAWLER_LOG_H
#define _CRAWLER_LOG_H

#include "crawler_time_alarm.h"
#include "crawler_config.h"
#include <fstream>
#include <string>

using std::ofstream;
using std::string;

class crawler_log
{
public:
    //构造函数
    crawler_log();
    //带有参数的构造函数
    crawler_log(const std::string&);
    //析构函数
    ~crawler_log();
    //初始化日志对象
    int log_init(const std::string&);
    //打印流程日志
    void LOG_WRITE_PROCESS(const std::string&,...);
    //打印错误日志
    void LOG_WRITE_ERROR(const std::string&,...); 
    //打印系统日志
    void LOG_WRITE_SYS(const std::string&,...);
private:
    crawler_time_alarm _log_alarm;
    crawler_config _log_config;
    ofstream _log_file;
    string _current_log_time;
    string LOG_PATH;
    string PREFIX;
    string LAST_LOG_TIME;
    int INTERVAL_TIME;
    int _switch_time;
    enum LEVEL{
        SYS = -2,
        ERROR = -1,
        PROCESS = 0
     } level;
    enum COLOR{
        GREEN = 32,
        RED = 31,
        YELLOW = 33,
        BLUE = 34,
        PURPLE = 35
    } color;

    void _constructor();
    void _read_config(const string&);
    void _write_log(int,const string&,const int color);
    int _log_check();
    bool _log_set_timeout();
    static void* _log_switch_fn(void*);
};

#endif


