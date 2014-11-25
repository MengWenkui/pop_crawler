/**
 * 网络类
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
#include "crawler_thread_pool.h"
#include "crawler_log.h"
#include "crawler_bloom_filter.h"
#include "crawler_user_agent_pool.h"
#include "crawler_url_parse.h"

using std::string;
using std::map;

typedef void* (epoll_call_back)(const int,const int);
typedef void (thread_call_back)(void*);

struct socket_packet
{
    int socket_fd;
    string request;
    string url;
};

class crawler_net
{
public:
    crawler_net();
    ~crawler_net();
    int net_reload();
    int net_set_header(const string&,const string&);
    int net_http_get(const string&);
    int net_http_post(const string&,const string&);
private:
    crawler_user_agent_pool _agent_pool;
    crawler_bloom_filter url_bloom_filter;
    crawler_thread_pool work_thread_pool;
    crawler_url_parse url_parser;
    crawler_epoll request_epoll;
    crawler_epoll response_epoll;
    string http;
    map<string,string> header_map;
    int net_url_parse();
    int net_dns_parse(const string&,string&);
    int net_socket_init();
    epoll_call_back request_callback_func;
    epoll_call_back response_callback_func;
    void _set_default_header();
    thread_call_back thread_request_func;
    thread_call_back thread_response_func;
};

#endif
