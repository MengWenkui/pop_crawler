#ifndef _CRAWLER_URL_PARSE_H
#define _CRAWLER_URL_PARSE_H

#pragma once
#include <string>
#include <boost/regex.hpp>

class crawler_url_parse
{
public:
    std::string URL;
    std::string PROTOCOL;
    std::string DOMAIN;
    std::string PORT;
    std::string PATH;
    std::string PARAM;
    std::string IP;
    crawler_url_parse();
    crawler_url_parse(const std::string&);
    ~crawler_url_parse();
private:
    std::string reg_pattern;
    void url_parse_regex(const std::string&);
};

#endif
