#include "crawler_socket.h"
#include "crawler_epoll.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

crawler_epoll m_epoll;

void* call_back(const int sock,const int opt)
{
    cout << "套接字为" << sock << endl;

    if(opt == EV_ACCEPT)
    {
        vector<int> vec;
        vector<int>::iterator iter; 
        uint32_t option = EPOLLIN;
        crawler_socket::socket_accept(sock,vec);
        for(iter = vec.begin();iter != vec.end();iter++)
        {
            m_epoll.epoll_add(*iter,option);
        }
    }
    if(opt == EV_READ)
    {
        string buf;
        if(crawler_socket::socket_recv(sock,buf) == -1)
        {
            cout << "连接已经断开" << endl;
        }

        cout << "读取的数据" << buf << endl;
    }
    if(opt == EV_WRITE)
    {
        string buf = "测试写的数据";
        if(crawler_socket::socket_send(sock,buf,13))
        {
            cout << "发送失败了" << endl;
        }
    }
    if(opt == EV_DROP)
    {
        cout << "连接" << sock << "断开了" << endl;
    }

}

int main(int argc,char **argv)
{
    crawler_socket m_sock(60001);
    cout << "socket的值为" << m_sock.socket_fd << endl;

    
    if(m_epoll.epoll_init(&call_back))
    {
        cout << "epoll_init失败" << endl;
        return -1;
    }

    m_epoll.epoll_set_server(m_sock.socket_fd);

    uint32_t option = EPOLLIN;
    if(m_epoll.epoll_add(m_sock.socket_fd,option))
    {
        cout << "epoll_add失败" << endl;
        return -1;
    }

    if(m_epoll.epoll_run(-1))
    {
        cout << "epoll_run失败" << endl;
        return -1;
    }


    return 0;

}


