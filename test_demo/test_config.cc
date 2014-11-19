#include "../src/crawler_config.h"
#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main(int argv,char** argc)
{
    crawler_config m_conf("../config/user_agent_pool.config");
   
    map<string,vector<string> >::iterator iter = m_conf.config_map.begin();
    for(;iter != m_conf.config_map.end();iter++)
    {
        cout << "键值为：" << iter->first << endl;
        for(int i = 0; i < iter->second.size();i++)
        {
            cout << iter->second[i] << endl;
        }
    }
    
    return 0;
}
    
