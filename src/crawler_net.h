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
#include <vector>

class crawler_net
{
public:
    crawler_net();
    ~crawler_net();
private:
    std::vector<std::string> vec_header;
    int net_dns_parse(const std::string&,string&);
};

#endif
