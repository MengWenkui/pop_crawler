#include "crawler_net.h"
using namespace std;

crawler_net::crawler_net()
{
    //设置发送回调函数
    request_epoll.init(&request_callback_func);
    //设置接收回调函数
    response_epoll.init(&response_callback_func);
    //添加默认情况下的必要header
    header_map["Host"] = 网站域名;
    header_map["Referer"] = 带有协议的域名;
    header_map["User_Agent"] = 用户行I下（采用agent池机制），防down机制之一;
    header_map["Cache-Control"] = 缓存方式;
    header_map["Date"] = 当前时间;
    header_map["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"; 
    header_map["Accept-Encoding"] = "gzip,deflate";
    header_map["Accept-Language"] = "zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3";
}

crawler_net::~crawler_net()
{
    
}

int crawler_net::net_url_parse(const string& url)
{
    //调用crawler_url_parse类进行url解析
    return 0;
}

int crawler_net::net_dns_parse(const string& domain,string& host)
{
    struct hostent *p_host;
    char ip_addr[32] = {0};

    if(!p_host = gethostbyname(domain.c_str()))
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
            for(;*pp_addr_list != NULL;pp_addr++)
            {
                inet_ntop(p_host->h_addrtype,*pp_addr_list,ip_addr,sizeof(ip_adddr));
                printf("ip地址为：【%s】",ip_addr);
            }
            break;
        default:
            break;
    }

    return 0;
}

int crawler_net::net_http_get(const string& url,const string& param,const string& data)
{
    //1 解析url
    //2 对解析出来的域名进行dns
    //3 设置好相应的request的内容
    //4 建立连接并放入epoll
    //5 收到epoll回调，将任务加入到线程池中
    //  请求的任务执行完就回收线程资源，应答的任务执行完还要进行html解析，存入数据库，加入布隆过滤器等工作

    //通过get的方式进行数据发送,包括无数据
    string http_request = "GET" + url + data + "HTTP/1.1\r\n";
    string http_header;
            //添加手动设置的header，如果和默认的冲突，添加手动添加的
        vector<string>::iterator header_iter = vec_header.begin();
        for(;header_iter != vec_header.end();header_iter++)
        {
            http_header = *header_iter + "\r\n";
        }
    string http_data = data;
    string http = http_request + http_header + "\r\n" + http_http_data;
    

    
    return 0;   
}

int crawler_net::net_http_post(const string& http_ur,const string& data)
{
    //通过post的方式进行数据发送
    return 0;
}

int crawler_net::net_socket_init()
{
    //socket的初始化工作
    return 0;
}

void* request_callback_func(const int,const int)
{

}

void* response_callback_func(const int,const int)
{

}
