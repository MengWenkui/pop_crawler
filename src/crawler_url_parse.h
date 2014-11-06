#ifndef _CRAWLER_URL_PARSE_H
#define _CRAWLER_URL_PARSE_H

#pragma once
#include <string>
#include <regex>

class crawler_url_parse
{
public:
    std::string URL;
    std::string PROTOCOL;
    std::string HOST;
    std::string PORT;
    std::string PATH;
    std::string PARAM;
    crawler_url_parse();
    crawler_url_parse(const std::string&);
    ~crawler_url_parse();
private:
    void url_parse_protocol();
    void url_parse_host();
    void url_parse_port();
    void url_parse_path();
    void url_parse_param()
};

#endif
