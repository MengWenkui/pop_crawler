#include "stdafx.h"
#include "crawler_log.h"
#include "crawler_basic.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
using namespace std;

const int MAX_ARG_LEN = 1024;
const int MAX_INFO_LEN = 1256;

void crawler_log::_constructor()
{
     //日志切换间隔时间（小时）
    INTERVAL_TIME = 0;
    //切换剩余时间（秒)
    _switch_time = 0;
}

crawler_log::crawler_log()
{
    _constructor();
}

crawler_log::crawler_log(const string& config_path)
{
    _constructor();
    _read_config(config_path);
}

crawler_log::~crawler_log()
{
    _log_file.close();
    _log_alarm.time_alarm_destroy();
}

void crawler_log::_read_config(const string& config_path)
{
    //读取配置文件信息
    _log_config.config_init(config_path);
    //日志路径
    LOG_PATH = _log_config.config_map["path"].at(0);
    //日志切换间隔时间
    string interval_time = _log_config.config_map["interval_time"].at(0);
    INTERVAL_TIME = atoi(interval_time.c_str());
    //最后记录日志的时间串
    LAST_LOG_TIME = _log_config.config_map["last_log_time"].at(0);

    if(_log_set_timeout())
    {
        printf("\033[1;%dm设置日志切换时间失败，错误码【%s】\033[0m\n",err_code);
        return;
    }

}

int crawler_log::log_init(const string& config_path)
{
    _read_config(config_path);

    return 0;
}

//设置日志切换时间
//返回值：true成功，false失败
bool crawler_log::_log_set_timeout()
{
    //判断当前应该写入的日志文件
    if(_log_check() == -1)
    {
        return false;
    }
    //设置日志切换的定时器
    if(_log_alarm.time_alarm_set(_switch_time,0,&_log_switch_fn,this))
    {
        return false;
    }

    if(_log_alarm.time_alarm_create())
    {
        return false;
    }

    return true;
}

//判断应该打开的日志文件
//返回值：0打开最后一次打开的日志文件，1创建一个新的日志文件，-1打开最后一次打开的日志文件失败，-2创建新的日志文件失败
int crawler_log::_log_check()
{
    struct tm *t;
    crawler_basic::get_current_time(t);
    char expire_time[11] = {0};
    string full_log_path;
    int switch_hour = 0,switch_minute = 0;
    //获取当前系统的时间
    switch_minute = 60 - t->tm_min;

    //判断是否应该创建新的日志文件
    if((switch_hour = (abs(atoi(LAST_LOG_TIME.c_str()) - atoi(expire_time)))) <  INTERVAL_TIME)
    {
        //打开最后一次打开的日志文件
        full_log_path = LOG_PATH + "/crawler_log" + LAST_LOG_TIME + ".log";
        _log_file.open(full_log_path.c_str(), ofstream::out | ofstream::app);
        if(_log_file.bad())
        {
            printf("\033[1;%dm打开日志文件失败，文件路径【%s】\033[0m\n",full_log_path.c_str());
            return -1;
        }
        _switch_time = switch_hour*60*60 + switch_minute*60;
        _current_log_time = LAST_LOG_TIME;
        //在配置文件中写入最后打开的日志
        _log_config.config_write("last_log_time",_current_log_time);
        return 0;
    }

    snprintf(expire_time,11,"%4d%02d%02d%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour);
    full_log_path = LOG_PATH + "/crawler_log" + expire_time + ".log";
    _log_file.open(full_log_path.c_str(),ofstream::out | ofstream::app);

    if(_log_file.bad())
    {
        printf("\033[1;%dm创建日志文件失败，文件路径【%s】\033[0m\n",full_log_path.c_str());
        return -2;
    }

    _switch_time = INTERVAL_TIME*60*60;
    _current_log_time = expire_time;
    _log_config.config_write("last_log_time",_current_log_time);

    return 1;
}

void crawler_log::_write_log(int level,const string& arg_info,const int color)
{
    char log_info[MAX_INFO_LEN] = {0}; 

    struct tm *time;
    crawler_basic::get_current_time(time);

    snprintf(log_info,sizeof(log_info)-1,"\033[1;%dm%4d年%02d月%02d日%02d:%02d:%02d\t%d\t%s\033[0m\n",
            color,
            time->tm_year+1900,
            time->tm_mon+1,
            time->tm_mday,
            time->tm_hour,
            time->tm_min,
            time->tm_sec,
            level,
            arg_info.c_str());

    _log_file.write(log_info,strlen(log_info));

    _log_file.flush();
}

//打印流程日志
void crawler_log::LOG_WRITE_PROCESS(const string& format,...)
{
    level = PROCESS;
    color = GREEN;
    char arg_info[MAX_ARG_LEN] = {0};
    //获取不确定个参数
    va_list list;
    va_start(list,format);
    vsnprintf(arg_info,sizeof(arg_info)-1,format.c_str(),list);
    va_end(list);
    _write_log((int)level,arg_info,color);
}

//打印错误日志
void crawler_log::LOG_WRITE_ERROR(const string& format,...)
{
    level = ERROR;
    color = RED;
    char arg_info[MAX_ARG_LEN] = {0};
    va_list list;
    va_start(list,format);
    vsnprintf(arg_info,sizeof(arg_info)-1,format.c_str(),list);
    va_end(list);
    _write_log((int)level,arg_info,color);

}

//打印系统日志
void crawler_log::LOG_WRITE_SYS(const string& format,...)
{
    level = SYS;
    color = YELLOW;
    char arg_info[MAX_ARG_LEN] = {0};
    va_list list;
    va_start(list,format);
    vsnprintf(arg_info,sizeof(arg_info)-1,format.c_str(),list);
    va_end(list);
    _write_log((int)level,arg_info,color);

}

//日志切换定时动作
void* crawler_log::_log_switch_fn(void* obj)
{
    crawler_log *para = (crawler_log*)obj;

    //关闭原来的文件
    para->_log_file.close();
    //更新当前的日志时间
    char current_time[11] = {0};
    crawler_basic::itoa(atoi(para->_current_log_time.c_str()) + para->INTERVAL_TIME,current_time);
    para->_current_log_time = current_time;
    string log_full_path = para->LOG_PATH + "/crawler_log"+current_time + ".log";
    para->_log_file.open(log_full_path.c_str());

    if(para->_log_file.bad())
    {
        printf("创建%s日志文件失败",log_full_path.c_str());
    }

    para->_log_config.config_write("last_log_time",para->_current_log_time);
    para->_switch_time = para->INTERVAL_TIME*60*60;
    para->_log_alarm.time_alarm_set(para->_switch_time,0,&_log_switch_fn,(void*)para);
}




