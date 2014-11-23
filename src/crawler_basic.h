/**
 * 基础类
 * 作者：zhangchunyang
 * 创建时间：2014-11-21
 */
#pragma once 
#ifndef _CRAWLER_BASIC_H
#define _CRAWLER_BASIC_H

#include <time.h>

class crawler_basic
{
public:
    crawler_basic();
    ~crawler_basic();
    static int itoa(int,char*);
    static int get_current_time(struct tm*&);
private:
};

#endif
