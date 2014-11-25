/**
 * 线程池类
 * 作者：张春阳
 * 创建时间：2014-11-4
 */
#ifndef _CRAWLER_THREAD_POOL_H
#define _CRAWLER_THREAD_POOL_H

#include <pthread.h>
#include <queue>
#include <vector>

using std::vector;
using std::priority_queue;

//任务队列
struct job
{
    void (*callback_func)(void *arg);
    void *arg;
    unsigned int level;
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
    crawler_thread_pool();
    crawler_thread_pool(const int);
    ~crawler_thread_pool();
    int thread_pool_init(const int);
    int thread_pool_add_job(void (*)(void*),void*);
    int thread_pool_destroy();
    private:
    bool _pool_close_flag;
    unsigned int _thread_num;
    unsigned int _queue_max_num;
    pthread_mutex_t _mutex;
    pthread_t *_p_threads;
    pthread_cond_t _queue_empty;
    pthread_cond_t _queue_not_empty;
    pthread_cond_t _queue_not_full;
    priority_queue<job,vector<job>,job_cmp> _job_queue; 
    
    void _constructor();
    int _create_thread_pool(const unsigned int);
    static void* _thread_func(void *p)
    {
        static_cast<crawler_thread_pool*>(p)->_thread_pool_func(p);
    }
    void _thread_pool_func(void*);
};

#endif
