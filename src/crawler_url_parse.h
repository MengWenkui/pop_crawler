#ifndef _CRAWLER_URL_PARSE_H
#define _CRAWLER_URL_PARSE_H

#pragma once
#include <string>
#include <boost/regex.hpp>
#include <map>

class crawler_url_parse
{
public:
    std::string URL;
    std::string PROTOCAL;
    std::string HOST;
    int PORT;
    std::string PATH;
    //std::map<std::string,std::string> PARAM;
    std::string PARAM;
    crawler_url_parse();
    ~crawler_url_parse();
    int url_parse(const std::string&);
private:
    std::string reg_pattern;
    void url_param_parse(std::string&);
};

#endif
