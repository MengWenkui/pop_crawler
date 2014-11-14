#include "crawler_socket.h"
#include "crawler_epoll.h"
#include <iostream>
#include <string>
using namespace std;

crawler_socket m_sock(60001);

void* call_back(const int sock,const int opt)
{
    cout << "进来了" << endl;
    string buf;
    m_sock.socket_recv(buf);
    cout << buf << endl;
}

int main(int argc,char **argv)
{
    cout << "socket的值为" << m_sock.socket_fd << endl;

    crawler_epoll m_epoll;
    
    if(m_epoll.epoll_init(&call_back))
    {
        cout << "epoll_init失败" << endl;
        return -1;
    }

    uint32_t option = EPOLLIN | EPOLLOUT;
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


