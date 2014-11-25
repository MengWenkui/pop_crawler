#include "crawler_epoll.h"
using namespace std;

//最大连接数
const int MAX_CONNECTION = 30;

crawler_epoll::crawler_epoll(const int type)
{
    //epoll类型
    epo_type = type;
    epoll_event_callback = NULL;
    //创建epoll
    epo_fd = epoll_create(MAX_CONNECTION + 1);
    //分配事件池大小
    events = new epoll_event[MAX_CONNECTION +1];
}

crawler_epoll::~crawler_epoll()
{
    //销毁事件池
    if(events != NULL)
    {
        delete [] events;
        events = NULL;
    }

    //销毁epoll
    if(epo_fd > 0)
    {
        close(epo_fd);
    }
    
    epoll_event_callback = NULL;

}

int crawler_epoll::epoll_init(void*(*func)(const int,const int))
{
    //初始化回调地址
    epoll_event_callback = func;
    return 0;
}

int crawler_epoll::epoll_set_server(const int server)
{
    server_fd = server;
    return 0;
}

//env为：
//EPOLLIN
//      相关联的文件对 read(2) 操作有效。
//EPOLLOUT
//      相关联的文件对 write(2) 操作有效。
//EPOLLRDHUP (从 Linux 2.6.17 开始)
//      流套接口对端关闭连接，或把写端关闭。(当使用边缘触发时这个标志可以简化检测对端关闭的代码。)
//EPOLLPRI
//      存在对 read(2) 操作有效的紧急数据。
//EPOLLERR
//      相关联的文件描述符发生错误状态。epoll_wait(2) 总是等待这个事件，它不需要在 events 中设置。
//EPOLLHUP
//      相关联的文件描述符延迟。epoll_wait(2) 总是等待这个事件，不需要在 events 中设置。
//EPOLLET
//      把相关联的文件描述符设置为边缘触发行为。对 epoll 默认的行为是水平触发的。参看 epoll(7) 来了解更多关于边缘和水平触发事件机制。
//EPOLLONESHOT (从 Linux 2.6.2 开始)
//      把相关联的文件描述符设置为一次性有效。这就是说在 epoll_wait(2) 抛出一个事件之后相关联的文件描述符将不再报告其他事件。用户必须使用 EPOLL_CTL_MOD 调用 epoll_ctl() 来重新设置文件描述符的新的事件掩码。 
//
//errno为：
//EBADF
//      epfd 或 fd 不是一个有效的文件描述符。
//EEXIST
//      op 是 EPOLL_CTL_ADD，并且提供的文件描述符 fd 已经存在于 epoll 实例中。
//EINVAL
//      epfd 不是一个 epoll 文件描述符，或者 fd 与 epfd 相同，或者请求的操作 op 不被本接口支持。
//ENOENT
//      op 是 EPOLL_CTL_MOD 或 EPOLL_CTL_DEL，但 fd 在相应的 epoll 实例中不存在。
//ENOMEM
//      没有足够的内存来处理请求的 op 控制操作。
//ENOSPC
//      试图注册(EPOLL_CTL_ADD)一个新文件描述符到一个 epoll 实例时达到了 /proc/sys/fs/epoll/max_user_watches 的限制。参看 epoll(7) 了解更多细节。
//EPERM
//      目标文件 fd 不被 epoll 支持。 
int crawler_epoll::epoll_add(const int socket_fd,const uint32_t env)
{
    if(!epoll_manage(socket_fd,env,EPOLL_CTL_ADD))
    {
        return -1;
    }

    return 0;
}

int crawler_epoll::epoll_remove(const int socket_fd)
{
    if(!epoll_manage(socket_fd,0,EPOLL_CTL_DEL))
    {
        return -1;
    }

    return 0;
}

int crawler_epoll::epoll_modify(const int socket_fd,const uint32_t env)
{
    if(!epoll_manage(socket_fd,env,EPOLL_CTL_MOD))
    {
        return -1;
    }

    return 0;
}

//EBADF
//      epfd 不是一个有效的文件描述符。
//EFAULT
//      events 指向的内存没有写权限。
//EINTR
//      在请求事件就绪或 timeout 到期之前，这个调用被信号处理器中断，参看 signal(7)。
//EINVAL
//      epfd 不是一个 epoll 文件描述符，或 maxevents 小于或等于零。 
int crawler_epoll::epoll_run(const int timeout)
{
    //循环获取事件
    while(1)
    {
        //等待事件（阻塞）
        int fd_counts = epoll_wait(epo_fd,events,MAX_CONNECTION+1,timeout);
       
        if((errno == EBADF || EFAULT || EINVAL) && fd_counts < 0)
        {
            return -1;
        }
        
        //轮询每个事件
        for(int i = 0;i < fd_counts;i++)
        {
            int op = 0;

            cout << "事件的代码" << events[i].events << endl;
            //错误事件和等待延时事件，暂时不做处理
            if(events[i].events == (EPOLLERR | EPOLLHUP))
            {
                cout << "1" << endl;
                continue;
            }
            //对方断开连接事件
            else if(events[i].events == (EPOLLRDHUP | EPOLLIN))
            {
                cout << "2" << endl;
                op = EV_DROP;
            }
            else if(events[i].data.fd == server_fd)
            {
                cout << "3" << endl;
                op = EV_ACCEPT;
            }
            //正常的读写事件
            else
            {
                cout << "4" << endl;
                if(events[i].events == EPOLLIN)
                {
                    op = EV_READ;
                }

                if(events[i].events == EPOLLOUT)
                {
                    op = EV_WRITE;
                }
                if(events[i].events == (EPOLLOUT | EPOLLHUP ))
                {
                    op = EV_WRITE;
                }
            }

            //回调函数进行读写处理
            (*epoll_event_callback)(events[i].data.fd,op);
        }

    }
    
    return 0;
            
}

//事件管理
bool crawler_epoll::epoll_manage(const int socket_fd,const uint32_t env,const int option)
{
    struct epoll_event event_tmp;
    event_tmp.data.fd = socket_fd;

    if(env == 0)
    {
        epoll_ctl(epo_fd,option,socket_fd,&event_tmp);
        return true;
    }

    if(epo_type == 0)
    {
        event_tmp.events |= env | EPOLLRDHUP;
    }

    if(epo_type == 1)
    {
        event_tmp.events |= EPOLLRDHUP;
        event_tmp.events |= env | EPOLLET;
    }

    if(epoll_ctl(epo_fd,option,socket_fd,&event_tmp))
    {
        return false;
    }

    return true;
}
