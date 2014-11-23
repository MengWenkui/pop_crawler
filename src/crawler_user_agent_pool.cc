#include "crawler_user_agent_pool.h"
#include "crawler_config.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

crawler_user_agent_pool::crawler_user_agent_pool()
{
    crawler_config agent_config("../config/user_agent_pool.config");
    _user_agent_pool = agent_config.config_map;
}

crawler_user_agent_pool::~crawler_user_agent_pool()
{

}

int crawler_user_agent_pool::agent_pool_get(string& agent_str)
{
    int count = 0;
    _get_rand_count(count);
    count = count % _user_agent_pool["user_agent"].size();
    agent_str = _user_agent_pool["user_agent"].at(count);
    
    return 0;
}

void crawler_user_agent_pool::_get_rand_count(int& count)
{
    srand((unsigned int)time(NULL));
    count = rand();
}
