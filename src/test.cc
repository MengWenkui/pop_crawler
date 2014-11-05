#include <iostream>
#include "crawler_config.h"
#include <map>
using namespace std;

int main(int argv,char** argc)
{
    crawler_config m_conf("配置文件路径");
   
    map<string,string>::iterator iter = m_conf.config_map.begin();
    for(;iter != m_conf.config_map.end();iter++)
    {
        cout << iter->first << "\t" << iter->second << endl;
    }
    
    cout << m_conf["key"] << endl;

    return 0;
}
     /*
    vector<pair<string,int> >::iterator iter = m_conf.config_index.begin();
    for(;iter != m_conf.config_index.end();iter++)
    {
        cout << iter->first << "\t" << iter->second << endl;
    }
    */
    //m_conf.config_write("who","zhangchunyang");
    //m_conf.config_write("love","o");
    //m_conf.config_write("this","爱");
    /*
    crawler_config m_conf;
    m_conf.config_init("../config/log.config");
    cout << m_conf.config_map["path"] << endl;
    */
    /*
    crawler_log m_log;
    m_log.log_init();
    for(int i = 0;i < 1000000;i++)
    {
        m_log.LOG_WRITE_PROCESS("FUNC%sENTER,MSG%s","我是一个函数名","这是一个很长的消息");
        m_log.LOG_WRITE_PROCESS("FUNC%sENTER,MSG%s","我是一个函数名","这是一个很长的消息");
        printf("hello\n");
        sleep(5);
    }
    */