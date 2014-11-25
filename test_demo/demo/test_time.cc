#include "../../src/crawler_time_alarm.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
using namespace std;

void call_back(void* arg)
{
    cout << *(char*)arg << endl;
}

int main(int argc,char **argv)
{
    crawler_time_alarm m_time(1,0,&call_back,(void*)"wokao",-1);
    m_time.time_alarm_create();
    sleep(10);
    return 0;
}

