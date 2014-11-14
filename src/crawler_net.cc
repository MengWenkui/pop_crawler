#include "crawler_net.h"
using namespace std;

crawler_net::crawler_net()
{
    //初始化epool，用于接收创建的socket的消息
    //计划是创建两个epool，分别用于接收发送线程池中和接收线程池中的消息完成数据接收的工作

}

crawler_net::~crawler_net()
{

}

int crawler_net::crawler_url_parse(const string& url)
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

int crawler_net::crawler_http_get(const string& url,const string& param,const string& data)
{
    //通过get的方式进行数据发送,包括无数据
    string http_request = "GET" + url + data + "HTTP/1.1\r\n";
    string http_header;
    if(vec_header.empty())
    {
    }
    else 
    {
        vector<string>::iterator header_iter = vec_header.begin();
        for(;header_iter != vec_header.end();header_iter++)
        {
            http_header = *header_iter + "\r\n";
        }
    }
    string http_data = data;
    string http = http_request + http_header + "\r\n" + http_http_data;
    

    
    return 0;   
}

int crawler_net::crawler_http_post(const string& http_ur,const string& data)
{
    //通过post的方式进行数据发送
    return 0;
}

int crawler_net::crawler_socket_init()
{
    //socket的初始化工作
    return 0;
}

