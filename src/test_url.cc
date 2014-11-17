#include <string>
#include <boost/regex.hpp>
#include <iostream>
using namespace std;
using namespace boost;

int main(int argc,char **argv)
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

     const string pattern = reg_protocal + "((?:" + reg_domain + "|" + reg_ip + "))" + reg_port + reg_path;

     string url = "http://www.google.com.hk/webhp?hl=zh-CN&sourceid=cnhp#safe=strict&hl=zh-CN&q=boost%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F";

     regex reg(pattern);

     cmatch matches;

     int counts = regex_match(url,matches,reg);

     if(counts > 0)
     {
         cout << "捕获的数量" << counts << endl;
         for(int i = 0;i < counts;i++)
         {
             cout << matches[i] << endl;
         }
     }

     return 0;
}
