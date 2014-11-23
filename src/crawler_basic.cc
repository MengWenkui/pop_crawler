#include "crawler_basic.h"
#include <stdio.h>

crawler_basic::crawler_basic()
{

}

crawler_basic::~crawler_basic()
{

}

int crawler_basic::itoa(int val,char *tmp)
{
    const unsigned int radix = 10;
    char *p = NULL,*b = NULL;
    unsigned int a = 0,u = 0;
    int len = 0;
    char temp;
     
    p = tmp;
     
    if(val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }

    u = (unsigned int)val;
     
    b = p;
 
    do
    {
        a = u%radix;
        u /= radix;
    }
    while(u > 0);
     
    len = (int)(p - tmp);
 
    *p-- = 0;
     
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;
    }
    while(b < p);

    return len;
}

int crawler_basic::get_current_time(struct tm *&t)
{
    time_t now = time(NULL);
    t = localtime(&now);
    return 0;
}
