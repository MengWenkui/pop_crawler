#include "crawler_socket.h"
#include <string>
#include <iostream>
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
using namespace std;

#define EVENT_MAX 20

int main(int argc,char **argv)
{
    crawler_socket server(10061);

    struct epoll_event event;
    struct epoll_event *events;

    int efd = epoll_create(EVENT_MAX);

    if(efd == -1)
    {
        cout << "create1" << endl;
        return -1;
    }

    event.data.fd = server.socket_fd;

    event.events = EPOLLIN | EPOLLET;

    int s = epoll_ctl(efd,EPOLL_CTL_ADD,server.socket_fd,&event);
    if(s == -1)
    {
        perror("epoll_ctl");
        return -1;
    }

    events = (epoll_event*)calloc(EVENT_MAX,sizeof(event));

    while(1)
    {
        int n,i;
        n = epoll_wait(efd,events,EVENT_MAX,-1);
        for(i = 0;i < n;i++)
        {
            if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN)))
            {
                perror("epoll event error");
                close(events[i].data.fd);
                continue;
            }
            else if(server.socket_fd == events[i].data.fd)
            {
                while(1)
                {
                    struct sockaddr in_addr;
                    socklen_t in_len;
                    int infd;
                    char hbuf[1024],sbuf[1024];

                    in_len = sizeof(in_addr);
                    infd = accept(server.socket_fd,&in_addr,&in_len);
                    if(infd == -1)
                    {
                        if((errno = EAGAIN) || (errno == EWOULDBLOCK))
                        {
                            break;
                        }
                        else
                        {
                            perror("accept");
                            break;
                        }
                    }

                    s = getnameinfo(&in_addr,in_len,hbuf,sizeof(hbuf),sbuf,sizeof(sbuf),NI_NUMERICHOST | NI_NUMERICSERV);
                    if(s == 0)
                    {
                        printf("accept connention on descriptor %d (host=%s,port=%s)\n",infd,hbuf,sbuf);

                    }
                }
                continue;
            }
            else
            {
                int done = 0;

                while(1)
                {
                    ssize_t count;
                    char buf[512];

                    count = read(events[i].data.fd,buf,sizeof(buf));
                    if(count == -1)
                    {
                        if(errno != EAGAIN)
                        {
                            perror("read");
                            done = 1;
                        }
                        break;
                    }
                    else
                    {
                        done =1;
                        break;
                    }

                }
                
                if(done)
                {
                    printf("connetion close");

                }

            }
        }
    }

    free(events);

    return 0;
}

            


