#include "../../src/crawler_log.h"
#include <iostream>
#include <unistd.h>
using namespace std;

int main(int argc,char **argv)
{
    crawler_log m_log("../config/log.config");
    string path = "../config/log.config";
    string func = "main";

    while(1)
    {
        m_log.LOG_WRITE_PROCESS("这是一个很长的测试语句，为了测试，只能很长%s%s",path.c_str(),func.c_str());
        sleep(5);
        m_log.LOG_WRITE_ERROR("这是一个错误的师范，为了测试而生");
        sleep(5);
        m_log.LOG_WRITE_SYS("这是一个系统的错误");
    }

    return 0;
}
