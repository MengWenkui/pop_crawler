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
#include <vector>
#include <limits.h>
#include "stdafx.h"


class crawler_socket
{
public:
    //套接字句柄
    int socket_fd;
    //构造函数
    crawler_socket();
    //带有参数的构造函数
    crawler_socket(const int,const int type = 0,const std::string& ip = "");
    //析构函数
    ~crawler_socket();
    //套接字初始化
    int socket_init(const int,const int type = 0,const std::string& ip = "");
    //接受连接
    static int socket_accept(const int,std::vector<int>&);
    //发送数据
    static int socket_send(const int,const std::string&,const int);
    //接收数据
    static int socket_recv(const int,std::string&);
    //关闭套接字
    void socket_close();
private:
    struct sockaddr_in socket_addr;
    int socket_set(const int,const int,const std::string&);
    static int socket_set_noblock(const int);
};

#endif


