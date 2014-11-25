#pragma once
#ifndef _CRAWLER_URL_PARSE_H
#define _CRAWLER_URL_PARSE_H

#include <string>
#include <boost/regex.hpp>
//#include <map>

using std::string;

class crawler_url_parse
{
public:
    string URL;
    string PROTOCAL;
    string HOST;
    int PORT;
    string PATH;
    //std::map<std::string,std::string> PARAM;
    string PARAM;
    crawler_url_parse();
    ~crawler_url_parse();
    int url_parse(const string&);
private:
    string reg_pattern;
    void url_param_parse(string&);
};

#endif
