#include "../../src/crawler_epoll.h"
#include "../../src/crawler_socket.h"
#include <iostream>
#include <string>
using namespace std;

crawler_epoll m_epoll;

void* call_back(const int sock,const int opt)
{
    if(opt == EV_WRITE)
    {
        cout << "进来了" << sock << opt << endl;
    }
}

int main(int argc,char **argv)
{
    crawler_socket m_client(60001,1,"127.0.0.1");
    m_epoll.epoll_init(&call_back);
    uint32_t option = EPOLLIN | EPOLLOUT;
    m_epoll.epoll_add(m_client.socket_fd,option);
    m_epoll.epoll_run(-1);
    return 0;
}
