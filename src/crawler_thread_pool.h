/**
 * 网络爬虫线程池类
 * 作者：张春阳
 * 创建时间：2014-11-4
 */
#ifndef CRAWLER_THREAD_POOL
#define CRAWLER_THREAD_POOL

#include <stdio.h>
#include <pthread.h>
#include <queue>
#include <vector>

//任务队列
struct job
{
    void* (*callback_func)(void *arg);
    void *arg;
    int level;
};

struct job_cmp
{
    bool operator() (job a,job b)
    {
        return b.level > a.level;
    }
};

class crawler_thread_pool
{
public:
    int err_code;
    crawler_thread_pool(const int thread_num = 10);
    ~crawler_thread_pool();
    int thread_pool_add_job(void* (*)(void*),void*);
    int thread_pool_destroy();
    pthread_mutex_t mutex;
    pthread_t *p_threads;
    pthread_cond_t queue_empty;
    pthread_cond_t queue_not_empty;
    pthread_cond_t queue_not_full;
    std::priority_queue<job,std::vector<job>,job_cmp> job_queue; 
    bool pool_close_flag;
    int queue_max_num;
private:
    int thread_num;
    static void* thread_pool_func(void*);
};

#endif
