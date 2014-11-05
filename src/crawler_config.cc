#include "crawler_config.h"
using namespace std;
//注释标识
#define COMMENT_MARK "#"
//空行标识
#define SPACE_MARK "*"
//每行最大长度
#define EACH_LINE_MAX 2048 

//无参数构造函数
crawler_config::crawler_config()
{
    config_map_counts = 0;
    err_code = 0;
}

//带有文件名参数的构造函数
crawler_config::crawler_config(const string& file_name)
{
    err_code = 0;

    if(file_name.empty())
    {
        err_code = 800;
        return;
    }

    config_map_counts = 0;
    err_code = 0;
    config_file_name = file_name;
    config_read_file.open(file_name.c_str());

    if(config_read_file.bad())
    {
        err_code = 801;
        printf("配置文件【%s】打开失败",file_name.c_str()); 
        return;
    }

    config_map_counts = config_generate_map();
}

//析构函数
crawler_config::~crawler_config()
{
    config_read_file.close();
}

//初始化配置文件对象,作用与带参数的构造函数相同
//返回值：成功返回0
int crawler_config::config_init(const string& file_name)
{
    int res = 0;

    if(file_name.empty())
    {
        err_code = 800;
        return err_code;
    }

    config_file_name = file_name;
    config_read_file.open(file_name.c_str());

    if(!config_read_file)
    {
        err_code = 801;
        printf("配置文件【%s】打开失败",file_name.c_str());
        return err_code;
    }

    if((config_map_counts = config_generate_map()) == -1)
    {
        err_code = 888;
        return err_code;
    }

    return res;  
}

//生成配置文件的map
//返回值：成功返回k-v的个数，失败返回-1
int crawler_config::config_generate_map()
{
    int map_count = 0;
    string each_line,no_comment_line,trim_key,trim_value;
    //按行获取文件
    while(getline(config_read_file,each_line))
    {
        //如果该行为空则跳过
        if(each_line.empty())
        {
            config_index.push_back(make_pair(SPACE_MARK,1));
            continue;
        }

        int pos =0,start_pos = 0,end_pos = each_line.size()-1;

        //查找注释标志的位置，如果在第1个则忽略该行，如果不是第1个则截取它前面的字符串
        if((pos = each_line.find(COMMENT_MARK)) != -1)
        {
            if(pos == 0)
            {
                config_index.push_back(make_pair(COMMENT_MARK,each_line.size()));
                continue;
            }
            end_pos = pos;
        }

        no_comment_line = each_line.substr(start_pos,start_pos + end_pos+1);

        //查找截取后"="的位置，如果在第1个则该行忽略，如果不是第1个则截取它前面的字符串
        if((pos = no_comment_line.find('=')) == -1)
        {
            continue;
        }

        trim_key = no_comment_line.substr(0,pos);
        //去掉前后的空格
        //截取'='后面的字符串
        trim_value = no_comment_line.substr(pos+1,end_pos-pos);
        //去掉前后的空格
        config_trim(trim_value);
        config_trim(trim_key);
        //存入生成的config_map中
        config_map[trim_key] = trim_value;
        config_index.push_back(make_pair(trim_key,each_line.size()));
        //解析出的k-v对的个数
        map_count++;
        //将config_map_iter赋值
        config_map_iter = config_map.begin();
        each_line.clear();
    }

    if(map_count == 0)
    {
        return -1;
    }

    return map_count;
}

//去掉字符串的前后空格
//返回值：0成功，-1字符串为空，-2字符串都是空格
int crawler_config::config_trim(string& trim_str)
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
        if(!config_is_space(trim_str[i]))
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
        if(!config_is_space(trim_str[i]))
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

//判断字符是否为空格
//返回值：空格true，不是空格false
bool crawler_config::config_is_space(char c)
{
    if(c == ' ' || c == '\t')
    {
        return true;
    }

    return false;
}

//写入配置文件
//返回值：0添加配置项，1更新配置项，801系统错误
int crawler_config::config_write(const string& key,const string& value)
{
    int res = 0;

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
    config_write_file.open(config_file_name.c_str());
    //搜索是否已经存在
    vector<pair<string,int> >::iterator find_iter;
    for(find_iter = config_index.begin();find_iter != config_index.end();find_iter++)
    {
        if(find_iter->first == key)
        {
            origin_length = find_iter->second;
            break;
        }
    }
    
    //配置文件中已有该键值
    if(find_iter != config_index.end())
    {
        //键值的行号
        locate_line = distance(config_index.begin(),find_iter);
        //定位到写入位置
        for(int i = locate_line-1; i >= 0; i--)
        {
            write_offset += config_index.at(i).second; 
        }
        config_write_file.seekp(write_offset+1);
        
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
        
        config_write_file.write(config_item_tmp,strlen(config_item_tmp));
        config_write_file.close();
        config_map[key] = value;
        config_index[locate_line] = make_pair(key,strlen(config_item_tmp));
        res = 1;
        return res;
    }

    config_write_file.seekp(0,ios::end);
    if(config_map.empty())
    {
        snprintf(config_item_tmp,EACH_LINE_MAX,"%s",config_item.c_str());
    }
    else
    {
        snprintf(config_item_tmp,EACH_LINE_MAX,"\n%s",config_item.c_str());
    }
    config_write_file.write(config_item_tmp,strlen(config_item_tmp));
    config_write_file.close();
    config_map[key] = value;
    config_index.push_back(make_pair(key,strlen(config_item_tmp)));

    return res;
}
