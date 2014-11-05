#include <iostream>
#include "crawler_bloom_filter.h"
using namespace std;

int main(int argc,char **argv)
{
    crawler_bloom_filter m_bloom;
    m_bloom.bloom_add("爱");
    cout<< "______________________________________" << endl;
    if(m_bloom.bloom_exist("爱") == 1)
    {
        cout << "存在了，成功了一小步" << endl;
    }
    else
    {
        cout << "不存在" << endl;
    }
  
    
    m_bloom.bloom_add("www.baidu.com");
    m_bloom.bloom_add("www.google.com");
    m_bloom.bloom_add("www.sina.com");
   

    if(m_bloom.bloom_exist("www.google.com") == 1)
    {
        cout << "www.google.com 存在了" << endl;
    }
    else
    {
        cout << "www.google.com 不存在" << endl;
    }

    if(m_bloom.bloom_exist("www.office.com") == 1)
    {
        cout << "www.office.com 存在了" << endl;
    }
    else
    {
        cout << "www.office.com 不存在" << endl;
    }
        
    return 0;
}

