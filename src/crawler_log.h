/**
 * 爬虫日志类
 * 作者：张春阳
 * 创建日期：2014-10-24
 */
//#pragma once
#ifndef _CRAWLER_LOG_H
#define _CRAWLER_LOG_H

#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "crawler_config.h"
#include "crawler_time_alarm.h"

int itoa(int,char*);

class crawler_log
{
public:
    crawler_time_alarm log_alarm;
    crawler_config log_config;
    int INTERVAL_TIME;
    int switch_time;
    std::ofstream log_file;
    std::string current_log_time;
    std::string LOG_PATH;
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
    int err_code;
    std::string PREFIX;
    std::string LAST_LOG_TIME;
    enum LEVEL{
        SYS = -2,
        ERROR = -1,
        PROCESS = 0
     } level;
    int log_check();
    bool log_set_timeout();
    void get_current_time(struct tm*&);
    static void* log_switch_fn(void*);
};

#endif


