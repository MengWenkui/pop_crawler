/**
 * 爬虫网络类
 * 作者：张春阳
 * 创建时间：2014-10-30
 */
#ifndef CRAWLER_NET
#define CRAWLER_NET

#include <string>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <map>
#include "crawler_epoll.h"
#include "crawler_socket.h"
#include "crawler_thread_poll.h"
#include "crawler_log.h"

struct socket_packet
{
    int socket_fd;
    string request;
}

class crawler_net
{
public:
    crawler_net();
    ~crawler_net();
    int net_reload();
    int net_add_header();
    int net_http_get();
    int net_http_post();
private:
    string http;
    crawler_thread_poll work_thread_poll;
    crawler_url_parse url_parser;
    crawler_epoll request_epoll;
    crawler_epoll response_epoll;
    std::map<std::string,std::string> header_map;
    int net_url_parse();
    int net_dns_parse(const std::string&,string&);
    int net_socket_init();
    void* request_callback_func(const int,const int);
    void* response_callback_func(const int,const int);

};

#endif
