#include "crawler_net.h"
#include <iostream>
using namespace std;

crawler_net::crawler_net()
{
    //初始化布隆适配器
    url_bloom_filter.bloom_filter_init(1000000,0.001);
    //设置接收epoll的回调函数
    epoll_call_back* request_func = (epoll_call_back*)&crawler_net::request_callback_func;
    epoll_call_back* response_func = (epoll_call_back*)&crawler_net::response_callback_func;
    //设置发送回调函数
    request_epoll.epoll_init(request_func);
    //设置接收回调函数
    response_epoll.epoll_init(response_func);
    //初始化线程池
    work_thread_pool.thread_pool_init(30);
}

crawler_net::~crawler_net()
{
    
}

int crawler_net::net_set_header(const string& header_key,const string& header_value)
{
    header_map[header_key] = header_value;
    return 0;
}

int crawler_net::net_http_get(const string& url)
{
    //1 解析url
    url_parser.url_parse(url);
    //2 对解析出来的域名进行dns
    string ip;
    net_dns_parse(url_parser.HOST,ip);
    //3 设置好相应的request的内容
    //通过get的方式进行数据发送,包括无数据
    string http_request = "GET " + url_parser.PATH + url_parser.PARAM + " HTTP/1.1\r\n";

    _set_default_header();
    string http_header;
    //从header_map中取出header并放入http请求报文中
    map<string,string>::iterator header_iter = header_map.begin();
    for(;header_iter != header_map.end();header_iter++)
    {
        http_header += header_iter->first + ":" + header_iter->second + "\r\n";
    }
    string http_data;
    if(!url_parser.PARAM.empty())
    {
        http_data = http_data + url_parser.PARAM + "\r\n";
    }

    http = http_request + http_header + "\r\n" + http_data;
    printf("组织好的get报文\n%s\n",http.c_str());
    //4 建立连接并放入epoll
    if(!url_parser.PORT)
    {
        crawler_socket http_socket(url_parser.PORT,1,ip);
    }

    crawler_socket http_socket(80,1,ip);
    request_epoll.epoll_add(http_socket.socket_fd,EPOLLOUT | EPOLLIN); 
        
    request_epoll.epoll_run(10);
    return 0;   
}

int crawler_net::net_http_post(const string& http_url,const string& data)
{
    //通过post的方式进行数据发送

    return 0;
}

int crawler_net::net_dns_parse(const string& domain,string& host_ip)
{
    struct hostent *p_host;
    char ip_addr[32] = {0};

    if(!(p_host = gethostbyname(domain.c_str())))
    {
        perror("dns解析失败");
        return -1;
    }
    char **pp_addr_list;

    switch(p_host->h_addrtype)
    {
        case AF_INET:
        case AF_INET6:
            pp_addr_list = p_host->h_addr_list;
            for(;*pp_addr_list != NULL;pp_addr_list++)
            {
                inet_ntop(p_host->h_addrtype,*pp_addr_list,ip_addr,sizeof(ip_addr));
                printf("ip地址为：【%s】",ip_addr);
            }
            break;
        default:
            break;
    }

    return 0;
}

void crawler_net::_set_default_header()
{
    //添加默认情况下的必要header
    header_map["Host"] = url_parser.HOST;
    header_map["Referer"] = url_parser.PROTOCAL + "//:" + url_parser.HOST;
    _agent_pool.agent_pool_get(header_map["User_Agent"]); 
    header_map["Cache-Control"] = "no-cache";
    //header_map["Date"] = 当前时间;
    header_map["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"; 
    header_map["Accept-Encoding"] = "gzip,deflate";
    header_map["Accept-Language"] = "zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3";
   
}

void* crawler_net::request_callback_func(const int sock,const int opt)
{
    printf("收到EPOLL的回调\n");

    //5 收到epoll回调，将任务加入到线程池中
    if(EV_WRITE == opt)
    {
        struct socket_packet pack = {0};
        pack.socket_fd = sock;
        pack.request = http;
        pack.url = url_parser.URL;
        thread_call_back *thread_request = (thread_call_back*)&crawler_net::thread_request_func;
        work_thread_pool.thread_pool_add_job(thread_request,&pack);
    }

}

void crawler_net::thread_request_func(void* obj)
{
    printf("请求线程工作");
    socket_packet *para = (socket_packet*)obj;

    if(!crawler_socket::socket_send(para->socket_fd,para->request,para->request.size()))
    {
        //请求成功就将url放入bloom_filter
        url_bloom_filter.bloom_add(para->url);
        //成功了就退出，归还线程资源
    }


}

void* crawler_net::response_callback_func(const int sock,const int opt)
{

    thread_call_back *thread_response = (thread_call_back*)&crawler_net::thread_response_func;
    if(EV_READ == opt)
    {
        struct socket_packet pack ={0};
        pack.socket_fd = sock;
        pack.request = http;
        pack.url = url_parser.URL;
        work_thread_pool.thread_pool_add_job(thread_response,&pack);
    }
    
    if(EV_DROP == opt)
    {
        //断线处理
        //把套接字从epoll中删除
        //关闭套接字
    }
}

void crawler_net::thread_response_func(void* obj)
{
    socket_packet * para = (socket_packet*)obj;

    string response_str;

    if(!crawler_socket::socket_recv(para->socket_fd,response_str))
    {
        //接收成功，处理html
        //提取html中的url
        //向bloom_filter进行验证是否已经请求过
        //没有请求过则把url加入大队列中
        //提取html中用户感兴趣的部分，这部分由另一个类单独实现，实现用户定制
        //将提取出来感兴趣的部分存入数据库中
        //完成一次抓取
    }


}
