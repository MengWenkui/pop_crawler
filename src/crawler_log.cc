#include "crawler_log.h"
using namespace std;

//无参构造函数
crawler_log::crawler_log()
{
    //日志切换间隔时间（小时）
    INTERVAL_TIME = 0;
    //切换剩余时间（秒)
    switch_time = 0;
}

//带有参数构造函数
crawler_log::crawler_log(const string& config_path)
{
    //切换剩余时间（秒）
    switch_time = 0;
    //读取配置文件信息
    log_config.config_init(config_path);
    //日志路径
    LOG_PATH = log_config.config_map["path"];
    //日志切换间隔时间
    string interval_time = log_config.config_map["interval_time"];
    INTERVAL_TIME = atoi(interval_time.c_str());
    //最后记录日志的时间串
    LAST_LOG_TIME = log_config.config_map["last_log_time"];

    if(log_set_timeout())
    {
        printf("设置切换时间失败【%s】",err_code);
        return;
    }

}

//析构函数
crawler_log::~crawler_log()
{
    log_file.close();
    log_alarm.time_alarm_destroy();
}

//初始化日志类实例
int crawler_log::log_init(const string& config_path)
{
    //读取配置文件信息
    log_config.config_init(config_path);
    LOG_PATH = log_config.config_map["path"];
    string interval_time = log_config.config_map["interval_time"];
    INTERVAL_TIME = atoi(interval_time.c_str());
    //最后记录日志的时间串
    LAST_LOG_TIME = log_config.config_map["last_log_time"];

    if(!log_set_timeout())
    {
        err_code = 901;
        printf("设置切换时间失败【%s】",err_code);
        return err_code;
    }

    return 0;
}

//设置日志切换时间
//返回值：true成功，false失败
bool crawler_log::log_set_timeout()
{
    //判断当前应该写入的日志文件
    if(log_check() == -1)
    {
        return false;
    }
    //设置日志切换的定时器
    if(log_alarm.time_alarm_set(switch_time,0,&log_switch_fn,this))
    {
        return false;
    }

    if(log_alarm.time_alarm_create())
    {
        return false;
    }

    return true;
}

//判断应该打开的日志文件
//返回值：0打开最后一次打开的日志文件，1创建一个新的日志文件，-1打开最后一次打开的日志文件失败，-2创建新的日志文件失败
int crawler_log::log_check()
{
    struct tm *t;
    get_current_time(t);
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
        log_file.open(full_log_path.c_str(), ofstream::out | ofstream::app);
        if(log_file.bad())
        {
            printf("打开%s日志文件失败",full_log_path.c_str());
            return -1;
        }
        switch_time = switch_hour*60*60 + switch_minute*60;
        current_log_time = LAST_LOG_TIME;
        //在配置文件中写入最后打开的日志
        log_config.config_write("last_log_time",current_log_time);
        return 0;
    }

    snprintf(expire_time,11,"%4d%02d%02d%02d",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour);
    full_log_path = LOG_PATH + "/crawler_log" + expire_time + ".log";
    log_file.open(full_log_path.c_str(),ofstream::out | ofstream::app);

    if(log_file.bad())
    {
        printf("创建%s日志文件失败",full_log_path.c_str());
        return -2;
    }

    switch_time = INTERVAL_TIME*60*60;
    current_log_time = expire_time;
    log_config.config_write("last_log_time",current_log_time);

    return 1;
}

//打印流程日志
void crawler_log::LOG_WRITE_PROCESS(const string& format,...)
{
    char arg_info[1024] = {0};
    char log_info[1256] = {0};
    //获取不确定个参数
    va_list list;
    va_start(list,format);
    vsnprintf(arg_info,sizeof(arg_info)-1,format.c_str(),list);
    va_end(list);
    tm *tt;
    get_current_time(tt);
    snprintf(log_info,sizeof(log_info),"%4d年%02d月%02d日%02d:%02d:%02d\t%d\t%s\n",
            tt->tm_year+1900,
            tt->tm_mon+1,
            tt->tm_mday,
            tt->tm_hour,
            tt->tm_min,
            tt->tm_sec,
            level = PROCESS,
            arg_info);
    log_file.write(log_info,strlen(log_info));
    log_file.flush();
}

//打印错误日志
void crawler_log::LOG_WRITE_ERROR(const string& format,...)
{
    char arg_info[1024] = {0};
    char log_info[1256] = {0};
    va_list list;
    va_start(list,format);
    vsnprintf(arg_info,sizeof(arg_info)-1,format.c_str(),list);
    va_end(list);
    tm *tt;
    get_current_time(tt);
    snprintf(log_info,sizeof(log_info)-1,"%4d年%02d月%02d日%02d:%02d:%02d\t%d\t%s\n",
            tt->tm_year+1900,
            tt->tm_mon+1,
            tt->tm_mday,
            tt->tm_hour,
            tt->tm_min,
            tt->tm_sec,
            level = PROCESS,
            arg_info);
    log_file.write(log_info,strlen(log_info));
    log_file.flush();
}

//打印系统日志
void crawler_log::LOG_WRITE_SYS(const string& format,...)
{
    char arg_info[1024] = {0};
    char log_info[1256] = {0};
    va_list list;
    va_start(list,format);
    vsnprintf(arg_info,sizeof(arg_info)-1,format.c_str(),list);
    va_end(list);
    tm *tt;
    get_current_time(tt);
    snprintf(log_info,sizeof(log_info)-1,"%4d年%02d月%02d日%02d:%02d:%02d\t%d\t%s\n",
            tt->tm_year+1900,
            tt->tm_mon+1,
            tt->tm_mday,
            tt->tm_hour,
            tt->tm_min,
            tt->tm_sec,
            level = PROCESS,
            arg_info);
    log_file.write(log_info,strlen(log_info));
    log_file.flush();
}

void crawler_log::get_current_time(struct tm *&t)
{
    time_t now = time(NULL);
    t = localtime(&now);
}

//日志切换定时动作
void* crawler_log::log_switch_fn(void* obj)
{
    crawler_log *para = (crawler_log*)obj;

    //关闭原来的文件
    para->log_file.close();
    //更新当前的日志时间
    char current_time[11] = {0};
    itoa(atoi(para->current_log_time.c_str()) + para->INTERVAL_TIME,current_time);
    para->current_log_time = current_time;
    string log_full_path = para->LOG_PATH + "/crawler_log"+current_time + ".log";
    para->log_file.open(log_full_path.c_str());

    if(para->log_file.bad())
    {
        printf("创建%s日志文件失败",log_full_path.c_str());
    }

    para->log_config.config_write("last_log_time",para->current_log_time);
    para->switch_time = para->INTERVAL_TIME*60*60;
    para->log_alarm.time_alarm_set(para->switch_time,0,&log_switch_fn,(void*)para);

}

int itoa(int val,char *tmp)
{
    const unsigned int radix = 10;
    char *p = NULL,*b = NULL;
    unsigned int a = 0,u = 0;
    int len = 0;
    char temp;

    p = tmp;

    if(val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }

    u = (unsigned int)val;

    b = p;

    do
    {
        a = u%radix;
        u /= radix;
    }
    while(u > 0);

    len = (int)(p - tmp);

    *p-- = 0;

    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;
    }
    while(b < p);

    return len;
}



