
#include "sock_server.h"
void *  TCP_10_th_DFA(void * f_data)
{
    struct ssock_tcp{
	    int fd[10];
	    int state[10];
    } sock_tcp;    
    sock_tcp =* (struct ssock_tcp *) f_data;
int state = 0;// инициалировали в начало графа
    char buf[1024];
    int bytes_read;
	      printf("Start DFA:%s\n",buf);

    int ready,epfd,sock_count=10;
    struct epoll_event ev;
    struct epoll_event evlist[1];
    if((epfd=epoll_create(10))<0)// создаем на 10 дискрипторов
        perror("epol create:");
   for(int i=0;i<10;i++)// формируем epoll
   { 
     ev.data.fd=sock_tcp.fd[i];
     ev.data.u32=i;
     ev.events=EPOLLIN;
     if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock_tcp.fd[i], &ev) < 0)
        perror("epol add:");
   }

	while(sock_count>0)
             {

       if((ready = epoll_wait(epfd, evlist,1, 100))<=0)
            perror("select");
       else
       { 
             if(( bytes_read = recv(evlist[0].data.fd, buf, 1024, 0))<0)
            perror("receive");
	      printf("receive message:%s\n",buf);
	     switch (sock_tcp.state[evlist[0].data.u32]) {
        case 0:
		{
		if(strcmp(buf,"start")==0)
		sock_tcp.state[evlist[0].data.u32]=1;
		if( strcmp(buf,"inc")==0)
		sock_tcp.state[evlist[0].data.u32]=1;
		break;
	//	printf("rez cmp: %i",strcmp(buf,"inc"));
		}
        case 1:
		{
		if( strcmp(buf,"inc")==0)
		sock_tcp.state[evlist[0].data.u32]=3;
		if( strcmp(buf,"dec")==0)
		sock_tcp.state[evlist[0].data.u32]=2;
		break;
		}
        case 2:
		{
		if( strcmp(buf,"inc")==0)
		sock_tcp.state[evlist[0].data.u32]=4;
		break;
		}
        case 3:
		{
		if( strcmp(buf,"dec")==0)
		sock_tcp.state[evlist[0].data.u32]=2;
		break;
		}
        case 4:
		{
		if( strcmp(buf,"inc")==0)
		sock_tcp.state[evlist[0].data.u32]=3;
		if( strcmp(buf,"dec")==0)
		sock_tcp.state[evlist[0].data.u32]=1;
		if( strcmp(buf,"close")==0)
		sock_tcp.state[evlist[0].data.u32]=5;
		break;
		}
       // default:

        }
	      printf("state :%i\n",sock_tcp.state[evlist[0].data.u32]);
	      sprintf(buf,"%i\n",sock_tcp.state[evlist[0].data.u32]);
             if(( send(evlist[0].data.fd, buf, strlen(buf), 0))<0)
            perror("send");
	    sleep(1);
              }
	     }
//	    close(sock);
 pthread_exit(0);

}
