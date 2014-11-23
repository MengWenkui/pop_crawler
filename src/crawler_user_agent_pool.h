#pragma once
#ifndef _CRAWLER_USER_AGENT_POOL_H
#define _CRAWLER_USER_AGENT_POOL_H

#include <string>
#include <map>
#include <vector>

using std::map;
using std::string;
using std::vector;

class crawler_user_agent_pool
{
public:
    crawler_user_agent_pool();
    ~crawler_user_agent_pool();
    int agent_pool_get(string&);
private:
    map<string,vector<string> > _user_agent_pool;
    void _get_rand_count(int&);
};

#endif


