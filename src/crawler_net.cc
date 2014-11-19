#include "crawler_net.h"
using namespace std;

crawler_net::crawler_net()
{
    //初始化布隆适配器
    url_bloom_filter.bloom_filter_init(1000000,0.001);
    //设置发送回调函数
    request_epoll.init(&request_callback_func);
    //设置接收回调函数
    response_epoll.init(&response_callback_func);
    //初始化线程池
    work_thread_poll(30);
}

crawler_net::~crawler_net()
{
    
}

int crawler_net::net_set_header(const string& header_key,const string& header_value)
{
    header_map[header] = header_value;
    return 0;
}

int crawler_net::net_http_get(const string& url,const string& param,const string& data)
{
    //1 解析url
    url_parser.url_parse(url);
    net_url_parse(url);
    //2 对解析出来的域名进行dns
    string ip;
    net_dns_parse(url_parser.HOST,ip);
    //3 设置好相应的request的内容
    //通过get的方式进行数据发送,包括无数据
    string http_request = "GET" + url + data + "HTTP/1.1\r\n";
    string http_header;
    //从header_map中取出header并放入http请求报文中
    map<string,string>::iterator header_iter = header_map.begin();
    for(;header_iter != header_map.end();header_iter++)
    {
        http_header += header_iter->first + ":" + header_iter->second + "\r\n";
    }

    if(!url_parser.PARAM.empty())
    {
        string http_data += (url_parser.PARAM + "\r\n");
    }

    http = http_request + http_header + "\r\n" + http_data;

    //4 建立连接并放入epoll
    if(!url_parser.PORT)
    {
        crawler_socket http_socket(url_parser.PORT,1,ip);
    }

    crawler_socket http_socket(80,1,ip);
    request_epool.epoll_add(http_socket.socket_fd,EPOLLOUT | EPOLLIN); 
        
    return 0;   
}

int crawler_net::net_http_post(const string& http_ur,const string& data)
{
    //通过post的方式进行数据发送
    return 0;
}

int crawler_net::net_dns_parse(const string& domain,string& host_ip)
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

void crawler_net::set_default_header()
{
    //添加默认情况下的必要header
    header_map["Host"] = url_parser.HOST;
    header_map["Referer"] = url_parser.PROTOCAL + "//:" + url_parser.HOST;
    header_map["User_Agent"] = 用户行I下（采用agent池机制），防down机制之一;
    header_map["Cache-Control"] = 缓存方式;
    header_map["Date"] = 当前时间;
    header_map["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"; 
    header_map["Accept-Encoding"] = "gzip,deflate";
    header_map["Accept-Language"] = "zh-cn,zh;q=0.8,en-us;q=0.5,en;q=0.3";
   
}

void* crawler_net::request_callback_func(const int sock,const int opt)
{
    //5 收到epoll回调，将任务加入到线程池中
    if(EV_WRITE = opt)
    {
        struct socket_packet pack = {0};
        pack.socket_fd = sock;
        pack.request = http;
        work_thread_poll.thread_poll_add_job(thread_request_func,&pack);
    }

}

void* crawler_net::thread_request_func(void* obj)
{
    socket_packet *para = (socket_packet*)obj;

    if(!crawler_socket::socket_send(para->socket_fd,para->request,para->request.size()))
    {
        //请求成功就将url放入bloom_filter
        url_bloom_filter.bloom_filter_add(para->url_parser.URL);
        //成功了就退出，归还线程资源
    }


}

void* response_callback_func(const int sock,const int opt)
{
    if(EV_READ = opt)
    {
        work_thread_poll.thread_poll_add_job(thread_response_func,&sock);
    }
    
    if(EV_DROP = opt)
    {
        //断线处理
        //把套接字从epoll中删除
        //关闭套接字
    }
}

void* crawler_net::thread_response_func(void* obj)
{
    int *socket_fd = (int*)obj;

    string response_str;

    if(!crawler_socket::socket_send(socket_fd,response_str))
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
