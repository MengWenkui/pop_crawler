#pragma once
#ifndef _CRAWLER_SOCKET_H
#define _CRAWLER_SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>

class crawler_socket
{
public:
    //套接字句柄
    int socket_fd;
    int err_code;
    //构造函数
    crawler_socket();
    //带有参数的构造函数
    crawler_socket(const int,const int type = 0,const std::string& ip = "");
    //析构函数
    ~crawler_socket();
    //套接字初始化
    int socket_init(const int,const int type = 0,const std::string& ip = "");
    //发送数据
    int socket_send(const std::string&,const int);
    //接收数据
    int socket_recv(std::string&);
    //关闭套接字
    void socket_close();
private:
    struct sockaddr_in socket_addr;
    int socket_set_noblock();
    int socket_set(const int,const int,const std::string&);
};

#endif


