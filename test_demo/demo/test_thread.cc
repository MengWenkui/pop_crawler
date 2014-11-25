#include "../../src/crawler_thread_pool.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
using namespace std;

void work(void* msg)
{
    char *p = (char*) msg;
    printf("回调函数%s\n",p);
    sleep(1);
}

int main(int argv,char **argc)
{
    crawler_thread_pool m_thread(10);
    m_thread.thread_pool_add_job(work,(void*)"1");
    m_thread.thread_pool_add_job(work,(void*)"2");
    m_thread.thread_pool_add_job(work,(void*)"3");
    m_thread.thread_pool_add_job(work,(void*)"4");
    m_thread.thread_pool_add_job(work,(void*)"5");
    m_thread.thread_pool_add_job(work,(void*)"6");
    m_thread.thread_pool_add_job(work,(void*)"7");
    m_thread.thread_pool_add_job(work,(void*)"8");
    m_thread.thread_pool_add_job(work,(void*)"9");
    m_thread.thread_pool_add_job(work,(void*)"10");
    m_thread.thread_pool_add_job(work,(void*)"11");
    m_thread.thread_pool_add_job(work,(void*)"12");
    m_thread.thread_pool_add_job(work,(void*)"13");
    m_thread.thread_pool_add_job(work,(void*)"14");
    m_thread.thread_pool_add_job(work,(void*)"15");
    m_thread.thread_pool_add_job(work,(void*)"16");
    sleep(5);
    m_thread.thread_pool_destroy();
    return 0;
}
