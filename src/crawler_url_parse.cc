#include "crawler_url_parse.h"

using namespace std;
using namespace boost;

crawler_url_parse::crawler_url_parse()
{
    //协议匹配
    const string  reg_protocal = "(?:(?:(https?)://)?)";
    //域名匹配
    const string reg_domain = "(?:(?:[a-z0-9](?:[-a-z0-9]*[a-z0-9])?\\.)+(?:com|net|edu|biz|gov|org|in(?:t|fo)|(?-i:[a-z][a-z])))";
    //ip地址匹配
    const string reg_ip = "(?:[01]?\\d\\d?|2[0-4]\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[05])";
    //端口匹配
    const string reg_port = "(?::(\\d{1,5}))?";
    //路径匹配
    const string reg_path = "(?:(/[^\\?]*)?)";
    //参数匹配
    const string reg_parameter = "(?:\\?)?(.*)";
    //完整的url正则匹配
    reg_pattern = reg_protocal + "((?:" + reg_domain + "|" + reg_ip + "))" + reg_port + reg_path + reg_parameter;
}

crawler_url_parse::~crawler_url_parse()
{

}

int crawler_url_parse::url_parse(const string& url)
{
    string param;
    regex reg(reg_pattern);
    smatch matches;

    regex_search(url,matches,reg);

    URL = url;
    PROTOCAL = matches[1];
    HOST = matches[2]; 
    PORT = atoi(matches[3].str().c_str());
    
    if(0 == PORT)
    {
        PORT = 80;
    }

    PATH = matches[4];
    if(PATH.empty())
    {
        PATH = "/";
    }

    PARAM = matches[5]; 

    //url_param_parse(param);

    return 0;
}
/*
void crawler_url_parse::url_param_parse(string& param_str)
{
    int ipos = 0,dpos = 0;
    int spos = 0,epos = 0;

    do
    {
        spos = ipos;
        ipos = param_str.find("&",ipos);
        epos = ipos;
        string each_pair = param_str.substr(spos,epos-spos);
        dpos = each_pair.find("=");
        string key = each_pair.substr(0,dpos);
        string value = each_pair.substr(dpos+1,each_pair.size()-1-dpos);
        PARAM[key] = value;
        ipos++;
    }
    while(ipos);
}
*/

