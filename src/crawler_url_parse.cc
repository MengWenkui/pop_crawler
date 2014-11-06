#include "crawler_url_parse.h"
using namespace std;

crawler_url_parse::crawler_url_parse()
{

}

crawler_url_parse::crawler_url_parse(const string& url)
{
    URL = url;
    url_parse_protocal();
    url_parse_host();
    url_parse_port();
    url_parse_path();
    url_parse_param();
}

crawler_url_parse::~crawler_url_parse()
{

}

int crawler_url_parse::url_parse(const string& url)
{
    URL = url;
    url_parse_protocal();
    url_parse_host();
    url_parse_port();
    url_parse_path();
    url_parse_param();
}

void crawler_url_parse::url_parse_protocal()
{
   regex pattern("(^[mailto|ssh|ftp|https?])://");
   

}

void crawler_url_parse::url_parse_host()
{

}

void crawler_url_parse::url_parse_port()
{

}

void crawler_url_parse::url_parse_path()
{

}

void crawler_url_parse::url_parse_param()
{

}
