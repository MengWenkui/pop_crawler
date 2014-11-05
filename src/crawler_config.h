/**
 * 爬虫配置文件类
 * 作者：张春阳
 * 创建时间：2014-10-25
 */
#pragma once
#ifndef CRAWLER_CONFIG_H
#define CRAWLER_CONFIG_H

#include <stdio.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

class crawler_config
{
public:
    //错误码
    int err_code;
    //配置文件名
    std::string config_file_name;
    //配置文件map
    std::map<std::string,std::string> config_map;
    //配置文件key-value个数
    int config_map_counts;
    //无参数构造函数，需要单独init 
    crawler_config();
    //有参数构造函数，不需要init
    crawler_config(const std::string&);
    ~crawler_config();
    //初始化配置文件
    int config_init(const std::string&);
    //写入配置文件
    int config_write(const std::string&,const std::string&);
private:
    std::fstream config_read_file;
    std::fstream config_write_file;
    std::map<std::string,std::string>::iterator config_map_iter;
    std::vector<std::pair<std::string,int> > config_index;
    int config_generate_map();
    int config_trim(std::string&);
    bool config_is_space(char);
    bool config_is_comment(char);
};

#endif

