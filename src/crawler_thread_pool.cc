#include "crawler_thread_pool.h"
#include "stdafx.h"
#include <malloc.h>
#include <stdio.h>
using namespace std;

void crawler_thread_pool::_constructor()
{
   _thread_num = 0;
   _queue_max_num = 0;
}

crawler_thread_pool::crawler_thread_pool()
{
    _constructor();
}

//线程池构造函数
crawler_thread_pool::crawler_thread_pool(const int _thread_num)
{
    _constructor();
    _create_thread_pool(_thread_num);
}

//线程池析构函数
crawler_thread_pool::~crawler_thread_pool()
{       
    //销毁线程池中的线程
    /* 
    for(int i = 0;i < _thread_num;i++)
    {
        pthread_join(_p_threads[i],NULL);
    }

    pthread_mutex_destroy(&_mutex);
    //销毁条件变量
    pthread_cond_destroy(&_queue_empty);
    pthread_cond_destroy(&_queue_not_full);

    pthread_cond_destroy(&_queue_not_empty);

    free(_p_threads);
    */
}

int crawler_thread_pool::_create_thread_pool(const unsigned int threads)
{
    do
    {
        //线程池中线程的数量
        _thread_num = threads;
        //队列任务数
        _queue_max_num = 20;
        //初始化互斥量
        if(pthread_mutex_init(&(_mutex),NULL))
        {
            err_code = -1;
            perror("init线程池互斥量失败");
            break;
        }
        //初始化条件变量
        //队列为空条件变量
        if(pthread_cond_init(&(_queue_empty),NULL))
        {
            err_code = -2;
            perror("init线程池条件变量失败");
            break;
        }
        //队列不为空条件变量
        if(pthread_cond_init(&(_queue_not_empty),NULL))
        {
            err_code = -2;
            perror("init线程池条件变量失败");
            break;
        }
        //队列不满条件变量
        if(pthread_cond_init(&(_queue_not_full),NULL))
        {
            err_code = -3;
            perror("init线程池条件变量失败");
            break;
        }
        //申请线程池地址
        _p_threads = (pthread_t*)malloc(sizeof(pthread_t) * _thread_num);
        if(NULL == _p_threads)
        {
            err_code = -3;
            perror("malloc线程池失败");
            break;
        }
        //线程池关闭标志
        _pool_close_flag = false;
        //创建线程池，线程开始工作
        for(unsigned int i = 0;i < _thread_num;i++)
        {
            pthread_create(&_p_threads[i],NULL,&_thread_func,this);
        }
    }
    while(0);
 
    return 0;
}

int crawler_thread_pool::thread_pool_init(const int _thread_num)
{
    _constructor();
    _create_thread_pool(_thread_num);
    return 0;
}

//向线程队列中添加job
int crawler_thread_pool::thread_pool_add_job(void (*callback_func)(void *arg),void *arg)
{
    do
    {
        //检查参数
        if(!callback_func || !arg)
        {
            err_code = -1;
            perror("参数错误");
            break;
        }
        //互斥量上锁
        pthread_mutex_lock(&_mutex);
        //队列满等待线程通知添加工作
        while(_job_queue.size() == _queue_max_num && !_pool_close_flag)
        {
            pthread_cond_wait(&_queue_not_full,&_mutex);
        }
        //线程池关闭则退出
        if(_pool_close_flag)
        {
            err_code = -2;
            pthread_mutex_unlock(&_mutex);
            break;
        }
        //新的任务加入队列
        struct job *p_job = (struct job*)malloc(sizeof(struct job));
        if(NULL == p_job)
        {
            err_code = -3;
            pthread_mutex_unlock(&_mutex);
            break;
        }
        //向队列中添加job
        p_job->callback_func = callback_func;
        p_job->arg = arg;
        p_job->level = 1;
        //将job放入队列中
        _job_queue.push(*p_job);
        //向所有线程广播，除非能够决定只有1个线程在当前的条件上，否则这种效率高
        pthread_cond_broadcast(&_queue_not_empty);
        pthread_mutex_unlock(&_mutex);
    }
    while(0);

    return 0;
}

//线程函数
void crawler_thread_pool::_thread_pool_func(void* arg)
{
    do
    {
        if(!arg)
        {
            perror("线程参数为空");
            break;
        }
        //获取实例中的变量
        crawler_thread_pool *para = (crawler_thread_pool*)arg;
        struct job top_job = {0};
        //只要线程池不被关闭则线程一直运行
        while(1)
        {
            pthread_mutex_lock(&para->_mutex);
            //当前队列中没有排队任务并且线程池没有关闭
            //线程会进入队列不满的条件状态中，等待外部唤醒
            //没有job的线程就处在这种状态中，自动释放互斥量，并等待触发条件
            //唤醒后的线程检查队列，并处理队列中的job
            while(para->_job_queue.empty() && !para->_pool_close_flag)
            {
                pthread_cond_wait(&para->_queue_not_empty,&para->_mutex);
            }
            //如果线程池关闭则线程退出
            if(para->_pool_close_flag)
            {
                pthread_mutex_unlock(&para->_mutex);
                pthread_exit(NULL);
            }
            //将队列中优先级最高的job弹出来
            top_job = para->_job_queue.top();
            para->_job_queue.pop();
            //队列为空时就可以发送销毁线程的条件了
            //如果已经调用销毁线程池则进行销毁否则不做任何操作
            if(para->_job_queue.empty())
            {
                pthread_cond_signal(&para->_queue_empty);
            } 
            //给队列发送不满的条件，让其填充值
            if(para->_job_queue.size() < para->_queue_max_num)
            {
                pthread_cond_broadcast(&para->_queue_not_full);
            }
            //解开互斥锁
            pthread_mutex_unlock(&para->_mutex);
            //线程处理job
            (*top_job.callback_func)(top_job.arg);
        }
    }
    while(0);

}

int crawler_thread_pool::thread_pool_destroy()
{
    do
    {
        pthread_mutex_lock(&_mutex);
        //如果线程池关闭，则直接退出
        if(_pool_close_flag)
        {
            err_code = -1;
            pthread_mutex_unlock(&_mutex);
            break;
        }
        
        //如果队列中还有job，等待队列为空的条件触发
        while(!_job_queue.empty())
        {
            pthread_cond_wait(&_queue_empty,&_mutex);
        }

        _pool_close_flag = true;

        pthread_mutex_unlock(&_mutex);
        //唤醒线程池中正在阻塞的job
        pthread_cond_broadcast(&_queue_not_empty);
        //唤醒添加job的函数
        pthread_cond_broadcast(&_queue_not_full);
        //等待其他线程结束
        //阻塞等待
        for(unsigned int i = 0;i < _thread_num;i++)
        {
            pthread_join(_p_threads[i],NULL);
        }
        pthread_mutex_destroy(&_mutex);
        //销毁条件变量
        pthread_cond_destroy(&_queue_empty);
        pthread_cond_destroy(&_queue_not_full);
        pthread_cond_destroy(&_queue_not_empty);

        free(_p_threads);
        _p_threads = NULL;
    }
    while(0);
    
    return 0;
}
