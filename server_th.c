
#include "sock_server.h"
void *  TCP_th(void * f_data)
{
int sock =* (int*) f_data;
    char buf[1024];
    int bytes_read;
	while(1)
             {
             if(( bytes_read = recv(sock, buf, 1024, 0))<0)
              perror("recv");
	      printf("receive message:%s",buf);
             if(( send(sock, buf, bytes_read, 0))<0)
            perror("send");
	    sleep(1);
              }
	    close(sock);
 pthread_exit(0);
}
