#include "crawler_url_parse.h"
using namespace std;
using namespace boost;

crawler_url_parse::crawler_url_parse()
{
   
    

}

crawler_url_parse::crawler_url_parse(const string& url)
{
    URL = url;
    url_parse_regex(url);
}

crawler_url_parse::~crawler_url_parse()
{

}

int crawler_url_parse::url_parse(const string& url)
{
    URL = url; 
}

void crawler_url_parse::url_parse_regex(const string& url)
{
    //协议匹配
    const string  reg_protocal = "(?:(^https?)://)";
    //域名匹配
    const string reg_domain = "(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+(?:com|net|edu|biz|gov|org|in(?:t|fo)|(?-i:[a-z][a-z]))";
    //ip匹配
    const string reg_ip = "(?:[01]?\\d\\d?|2[0-4]\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[05])";
    //端口号匹配
    const string reg_port = "(?::(\\d{1,5}))?";
    //路径匹配
    const string reg_path = "/((?:[a-zA-Z0-9]+)*)";
    //参数匹配
    const string reg_parameter = "";

    reg_pattern = reg_protocal + "((?:" + reg_domain + "|" + reg_ip "))" + reg_port + reg_path;

    url_parse_regex(url);

    if(

}

