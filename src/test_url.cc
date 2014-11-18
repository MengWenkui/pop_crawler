#include <string>
#include <iostream>
#include "crawler_url_parse.h"
#include <map>
using namespace std;
using namespace boost;

int main(int argc,char **argv)
{
    crawler_url_parse m_parse;
    m_parse.url_parse(argv[1]);
    cout << "URL：" << m_parse.URL << endl;
    cout << "协议为：" << m_parse.PROTOCAL << endl;
    cout << "主机名：" << m_parse.HOST << endl;
    cout << "端口：" << m_parse.PORT << endl;
    cout << "路径：" << m_parse.PATH << endl;
    /*
    map<string,string>::iterator iter = m_parse.PARAM.begin();
    for(;iter != m_parse.PARAM.end();iter++)
    {
        cout << iter->first << "\t" << iter->second << endl;
    }
    */
     return 0;
}
