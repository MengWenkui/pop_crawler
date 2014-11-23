/**
 * 配置文件操作
 * 作者：张春阳
 * 创建时间：2014-10-25
 */
#pragma once
#ifndef _CRAWLER_CONFIG_H
#define _CRAWLER_CONFIG_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::fstream;

class crawler_config
{
public:
    //配置文件名
    string config_file_name;
    //配置文件map
    map<std::string,std::vector<string> > config_map;
    //无参数构造函数，需要单独init 
    crawler_config();
    //有参数构造函数，不需要init
    crawler_config(const string&);
    //析构函数
    ~crawler_config();
    //初始化配置文件
    int config_init(const string&);
    //写入配置文件
    int config_write(const string&,const string&);
private:
    fstream _read_file_fd;
    fstream _write_file_fd;
    vector<pair<string,int> > _index;

    void _trim_solon(string&);
    void _split_value(const string&,vector<string>&);
    int _file_open(const string&);
    bool _judge_key_validate(const string&);
    int _split_comment(const string&,string&);
    int _split_assign(const string&,string&,string&);
    void _constructor();
    int _generate_map();
    int _trim_space(string&);
    bool _is_space(char);
    bool _is_comment(char);
};

#endif

