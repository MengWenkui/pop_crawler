#include "crawler_socket.h"
using namespace std;

//html最大长度
const int HTML_MAX_LEN = 1024*1024;
//每次接收的最大长度
const int  EACH_RECV_LEN = 1024;

crawler_socket::crawler_socket()
{
    socket_fd = 0;
    socket_addr = {0};
}

crawler_socket::crawler_socket(const int port,const int type,const string& ip)
{
    socket_fd = 0;
    socket_addr = {0};
    //设置套接字
    socket_set(type,port,ip);
}

crawler_socket::~crawler_socket()
{
    close(socket_fd);
}

int crawler_socket::socket_init(const int port,const int type,const string& ip)
{
    //设置套接字
    if(socket_set(type,port,ip))
    {
        err_code = 1000;
        return err_code;
    }
    return 0;
}

void crawler_socket::socket_close()
{
    close(socket_fd);
}

int crawler_socket::socket_send(const string& data,const int len)
{

    int written_len = 0;
    int expect_len = len;

    //循环发送，知道全部发送完毕 
    while(len)
    {
        int n = write(socket_fd,data.c_str() + written_len,expect_len);

        //发送失败
        if(n < 0)
        {
            //重试，可能缓冲区满
            if(errno == EAGAIN)
            {
                usleep(1000);
                continue;
            }
            
            err_code = 1008;
            return err_code;
        }

        written_len += n;
        expect_len -= n;
    }

    return 0;

}

int crawler_socket::socket_recv(string& data)
{
    int read_len = 0;
    char *read_data = new char[HTML_MAX_LEN];

    //循环读取，直到全部读取完毕
    while(1)
    {
        cout << "读取" << endl;
        int n = read(socket_fd,read_data + read_len,EACH_RECV_LEN);
        cout << "读取长度" << n << endl;
        cout << "错误码" << errno << endl;

        //失败
        if(n < 0)
        {
            //重试，可能是缓冲区已满
            //可能阻塞
            //信号打断
            if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
            {
                usleep(100000);
                continue;
            }
            
            err_code = 1008;
            return err_code;
        }
        //读取完
        else if(n == 0)
        {
            data = read_data;
            return 0;
        }
        //没有读取完
        else
        {
            read_len +=n;
        }
    }

    return 0;
}

//设置非阻塞
int crawler_socket::socket_set_noblock()
{
    int flag = 0;

    if((flag = fcntl(socket_fd,F_GETFL,0)) == -1)
    {
        return -1;
    }

    //非阻塞标志
    flag |= O_NONBLOCK;

    if(fcntl(socket_fd,F_SETFL,flag) == -1)
    {
        return -2;
    }
    
    return 0;

}

int crawler_socket::socket_set(const int type,const int port,const string& ip)
{
    //服务器类型
    if(type == 0)
    {
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = INADDR_ANY;
        socket_addr.sin_port = htons(port);

        //创建套接字
        if((socket_fd = socket(PF_INET,SOCK_STREAM,0)) < 0)
        {
			perror("创建错误");
            return -1;
        }
        
        //绑定套接字
        if(bind(socket_fd,(struct sockaddr*)&socket_addr,sizeof(struct sockaddr)) < 0)
        {
			perror("绑定错误");
            return -2;
        }

        //设置非阻塞
        if(socket_set_noblock())
        {
			perror("设置错误");
            return -3;
        }

        //监听端口，立刻返回，将套接字放入外部epool中，等待读写
        if(listen(socket_fd,5))
        {
			perror("监听错误");
            return -4;
        }

        
    }

    //客户端类型
    if(type == 1)
    {
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socket_addr.sin_port = htons(port);

        //创建套接字
        if((socket_fd = socket(PF_INET,SOCK_STREAM,0)) < 0)
        {
            return -1;
        }

        //设置套接字为非阻塞
        if(socket_set_noblock())
        {
            return -2;
        }

        //连接服务器，连接返回0，失败则立刻返回，将套接字放入外部的epool中，等待读写
        if(connect(socket_fd,(struct sockaddr*)&socket_addr,sizeof(struct sockaddr)) < 0)
        {
            return -3;
        }

    }

    return 0;
}
