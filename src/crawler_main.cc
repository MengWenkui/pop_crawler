#include "crawler_net.h"

int main(int argc,char **argv)
{
	crawler_net net;
    net.net_http_get("www.baidu.com");

	return 0;	
}
