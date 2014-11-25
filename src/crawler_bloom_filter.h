/**
 * 爬虫布隆过滤器类
 * 作者：张春阳
 * 创建时间：2014-10-31
 */

#pragma once
#ifndef _CRAWLER_BLOOM_FILTER_H
#define _CRAWLER_BLOOM_FILTER_H

#include <string>

using std::string;

class crawler_bloom_filter
{
public:
    //元素个数
    unsigned long long ele_counts;
    crawler_bloom_filter(const unsigned long long max_ele_num = 10000,const double false_positive = 0.01);
    ~crawler_bloom_filter();
    //布隆过滤器初始化
    int bloom_filter_init(const unsigned long long,const double);
    //检查布隆过滤器中是否存在值
    int bloom_exist(const string&);
    //向布隆过滤器中添加值
    int bloom_add(const string&);
private:
    int _hash_num;
    unsigned long long _size;
    unsigned _bit_block;
    unsigned int *_bit_map;
    int *_seeds;
    void _constructor(const unsigned long long,const double);
    bool _bit_jurge(const unsigned long long);
    int _hash_value(const string&,const int);
    bool _bit_set(const unsigned long long);
};

#endif
