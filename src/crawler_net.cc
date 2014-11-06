#include "crawler_net.h"
using namespace std;

crawler_net::crawler_net()
{

}

crawler_net::~crawler_net()
{

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

