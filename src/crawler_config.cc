#include "crawler_config.h"
#include "stdafx.h"
#include <stdio.h>
#include <algorithm>
#include <string.h>
using namespace std;

int err_code;

const string COMMENT_MARK = "#";
const string SPACE_MARK = "*";
const string ASSIGN_MARK = "=";
const char DOT_MARK = ','; 
const char COLON_MARK = '"';

#define EACH_LINE_MAX 2048 

void crawler_config::_constructor()
{

}

crawler_config::crawler_config()
{
    _constructor();
}

crawler_config::crawler_config(const string& file_name)
{
    _constructor();

    if(_file_open(file_name))
    {
        printf("生成配置文件失败，错误码【%d】",err_code);
    }

}

crawler_config::~crawler_config()
{
    _read_file_fd.close();
}

int crawler_config::_file_open(const string& file_name)
{
    if(file_name.empty())
    {
        err_code = 800;
        return -1;
    }

    config_file_name = file_name;
    _read_file_fd.open(file_name.c_str());

    if(!_read_file_fd)
    {
        err_code = 801;
        return -2;
    }

    if(_generate_map() == -1)
    {
        err_code = 802;
        return -3;
    }

    return 0;

}

int crawler_config::config_init(const string& file_name)
{
    if(_file_open(file_name))
    {
        printf("生成配置文件失败，错误码【%d】",err_code);
        return -1;
    }
    
    return 0;  
}

bool crawler_config::_judge_key_validate(const string& key)
{
    if(key[0] > '0' && key[0] < '9')
    {
        return false;
    }

    return true;
}

int  crawler_config::_split_comment(const string& each_line,string& nocomment_line)
{
    int mark_pos = 0;

    if((mark_pos = each_line.find(COMMENT_MARK)) != -1)
    {
        if(mark_pos == 0)
        {
            return 1;
        }
        else
        {
            nocomment_line = each_line.substr(0,mark_pos+1);
            return 2;
        }
    }
    
    _index.push_back(make_pair(COMMENT_MARK,each_line.size()));

    nocomment_line = each_line;

    return 3;
        
}

int crawler_config::_split_assign(const string& nocomment_line,string& trim_key,string& trim_value)
{
    int mark_pos = 0;

    if((mark_pos = nocomment_line.find(ASSIGN_MARK)) != -1)
    {
        if(mark_pos == 0)
        {
            return -2;
        }
        else
        {
            trim_key = nocomment_line.substr(0,mark_pos);
            trim_value = nocomment_line.substr(mark_pos+1,nocomment_line.size()-mark_pos);

            return 0;
        }
    }

     
    return -1;
}

void crawler_config::_split_value(const string& trim_value,vector<string>& vec_values)
{
    if(trim_value[0] != '{')
    {
        vec_values.push_back(trim_value);
        return;
    }


    string each_line;
    string whole_value;
    while(getline(_read_file_fd,each_line))
    {   
        if(_trim_space(each_line))
        {
             continue;
        }

        string nocomment_line;

        if(_split_comment(each_line,nocomment_line) == 1)
        {
            continue;
        }
        else
        {
            if(each_line[0] == '}')
            {
                break;
            }
            else
            {
                whole_value += each_line;
            }
        }
    }
        
    string sub_value;
    int false_mark_pos = 0,real_mark_pos = 0,start_pos = 0;

    while((false_mark_pos = whole_value.find(DOT_MARK,false_mark_pos)) != -1)
    {
        if(whole_value[false_mark_pos-1] == COLON_MARK)
        {
            real_mark_pos = false_mark_pos;
            sub_value = whole_value.substr(start_pos,real_mark_pos-start_pos);
            if(_trim_space(sub_value))
            {
                continue;
            }
            if(sub_value[sub_value.size()-1] == COLON_MARK)
            {
                start_pos = real_mark_pos + 1;
                false_mark_pos++;
                _trim_solon(sub_value);
                vec_values.push_back(sub_value);
            }
            else
            {
                false_mark_pos++;
            }
        }
        else
        {
            false_mark_pos++;
            continue;
        }
    }

    sub_value = whole_value.substr(real_mark_pos+1,whole_value.size()-real_mark_pos);
    _trim_solon(sub_value);
    vec_values.push_back(sub_value);
}

int crawler_config::_generate_map()
{
    string each_line;
    //按行获取文件
    while(getline(_read_file_fd,each_line))
    {
        //如果该行为空则跳过
        if(each_line.empty())
        {
            _index.push_back(make_pair(SPACE_MARK,1));
            continue;
        }
        
        string nocomment_line;
        
        if(_split_comment(each_line,nocomment_line) == 1)
        {
            continue;
        }
        
//printf((nocomment_line+"\n").c_str());

        string trim_key,trim_value;

        _index.push_back(make_pair(trim_key,each_line.size()));

        if(_split_assign(nocomment_line,trim_key,trim_value) != 0)
        {
            //发生错误是否停止
            continue;
        }
        
//printf((trim_key+"\n").c_str());

        if(_trim_space(trim_key))
        {
            //发生错误
            continue;
        }

        if(_trim_space(trim_value))
        {
            continue;
        }

        //判断key是否符合语法规范
        if(!_judge_key_validate(trim_key))
        {
            //不合法的情况该如何处理
            continue;
        }

        vector<string> vec_values;

        _split_value(trim_value,vec_values);

        //存入生成的config_map中
        config_map.insert(pair<string,vector<string> >(trim_key,vec_values));
        //解析出的k-v对的个数
        each_line.clear();
    }

    if(config_map.empty())
    {
        return -1;
    }

    return config_map.size();
}

int crawler_config::_trim_space(string& trim_str)
{
    //字符串是否为空
    if(trim_str.empty())
    {
        return -1;
    }

    //找到第1个字符串不为空的位置
    int i = 0,start_pos = 0,end_pos = 0;

    for(i = 0;i < trim_str.size();i++)
    {
        if(!_is_space(trim_str[i]))
        {
            break;        
        }
    }

    //如果第一个字符串不为空格的位置
    if(i == trim_str.size())
    {
        return -2;
    }

    //第1个不为空格的位置
    start_pos = i;

    //从后向前找字符不为空格的位置
    for(i = trim_str.size()-1; i >= start_pos; i--)
    {
        if(!_is_space(trim_str[i]))
        {
            break;
        }
    }

    //从后向前找字符不为空格的位置
    end_pos = i;
    //截取字符串
    trim_str = trim_str.substr(start_pos,end_pos-start_pos+1);

    return 0;
}

void crawler_config::_trim_solon(string& trim_str)
{
    if(trim_str[0] == '"')
    {
        trim_str.erase(0,1);
    }
    
    if(trim_str[trim_str.size()-1] == '"')
    {
        trim_str.erase(trim_str.size()-1,1);
    }
}

bool crawler_config::_is_space(char c)
{
    if(c == ' ' || c == '\t')
    {
        return true;
    }

    return false;
}

int crawler_config::config_write(const string& key,const string& value)
{
    int res = 0;
    vector<string> vec_key;

    if(key.empty() || value.empty())
    {
        err_code = 800;
        return err_code;
    }

    int write_offset = 0,origin_length = 0;
    int locate_line = 0;
    char config_item_tmp[EACH_LINE_MAX] = {0};
    //写入的配置项
    string config_item = key + " = " + value;
    _write_file_fd.open(config_file_name.c_str());
    //搜索是否已经存在
    vector<pair<string,int> >::iterator find_iter;
    for(find_iter = _index.begin();find_iter != _index.end();find_iter++)
    {
        if(find_iter->first == key)
        {
            origin_length = find_iter->second;
            break;
        }
    }
    
    //配置文件中已有该键值
    if(find_iter != _index.end())
    {
        //键值的行号
        locate_line = distance(_index.begin(),find_iter);
        //定位到写入位置
        for(int i = locate_line-1; i >= 0; i--)
        {
            write_offset += _index.at(i).second; 
        }
        _write_file_fd.seekp(write_offset+1);
        
        //如果写入位置在第一行
        if(locate_line == 0)
        {
            if(origin_length > config_item.size())
            {
                snprintf(config_item_tmp,EACH_LINE_MAX,"%-*s",origin_length,config_item.c_str());
            }
            else
            {
                snprintf(config_item_tmp,EACH_LINE_MAX,"%s",config_item.c_str());
            }
        }
        //写入位置不在第一行
        else
        {
            if(origin_length > config_item.size()+1)
            {
                snprintf(config_item_tmp,EACH_LINE_MAX,"\n%-*s",origin_length,config_item.c_str());
            }
            else
            {
                snprintf(config_item_tmp,EACH_LINE_MAX,"\n%s",config_item.c_str());
            }
        }
        
        _write_file_fd.write(config_item_tmp,strlen(config_item_tmp));
        _write_file_fd.close();
        vec_key.push_back(value);
        config_map.insert(pair<string,vector<string> >(key,vec_key));
        _index[locate_line] = make_pair(key,strlen(config_item_tmp));
        res = 1;
        return res;
    }

    _write_file_fd.seekp(0,ios::end);
    if(config_map.empty())
    {
        snprintf(config_item_tmp,EACH_LINE_MAX,"%s",config_item.c_str());
    }
    else
    {
        snprintf(config_item_tmp,EACH_LINE_MAX,"\n%s",config_item.c_str());
    }
    _write_file_fd.write(config_item_tmp,strlen(config_item_tmp));
    _write_file_fd.close();
    vec_key.push_back(value);
    config_map.insert(pair<string,vector<string> >(key,vec_key));
    _index.push_back(make_pair(key,strlen(config_item_tmp)));

    return res;
}


