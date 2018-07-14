
#include "sock_server.h"
void *  TCP_10_th_DFA(void * f_data)
{
    struct ssock_tcp  sock_tcp;    
    memcpy(&sock_tcp,f_data,sizeof(sock_tcp));
   // sock_tcp =* (struct ssock_tcp *) f_data;
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
     ev.events=EPOLLIN;
     if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock_tcp.fd[i], &ev) < 0)
        perror("epol add:");

   }

while(sock_count>0)
  {
      int cur_sock=-1;
       if((ready = epoll_wait(epfd, evlist,1, 100))<=0)
            perror("select");
	if(ready>0)
	{
		for(int i=2;i<sock_count;i++)
			if(sock_tcp.fd[i-2]==evlist[0].data.fd)
				cur_sock=i-2;
	if((cur_sock<11)&&(cur_sock>-1))	
        if(evlist[0].data.fd==sock_tcp.fd[cur_sock])// Поступил пакет пользователя
	{
    char buf[1024];
    int bytes_read;
	      printf("receive message:%i sock_tcp=%i\n",evlist[0].data.fd,sock_tcp.fd[cur_sock]);
             if(( bytes_read = recv(evlist[0].data.fd, buf, 1024, 0))<0)
                perror("receive");
	     sock_tcp.state[cur_sock]=DFA_E(sock_tcp.state[cur_sock],buf);
	      printf("state :%i in connection: %i\n",sock_tcp.state[cur_sock],sock_tcp.fd[cur_sock]);
	      sprintf(buf,"%i\n",sock_tcp.state[cur_sock]);
	 //  sleep(1); 
             if(( send(evlist[0].data.fd, buf, strlen(buf), 0))<0)
            perror("send");
	if(sock_tcp.state[cur_sock]==5) // надо завершить
	{
          if (epoll_ctl(epfd, EPOLL_CTL_DEL, sock_tcp.fd[cur_sock], NULL) < 0)
             perror("epol delete:");
	  close(sock_tcp.fd[cur_sock]);
	  printf("state %i in %i, close\n",sock_tcp.state[cur_sock],sock_tcp.fd[cur_sock]);
	  for(int i=cur_sock;i<sock_count-1;i++)// удаляем текущий сокет из массива
	  {
		  sock_tcp.fd[i]=sock_tcp.fd[i+1];
		  sock_tcp.state[i]=sock_tcp.state[i+1];
		  }
	  sock_count-=1;
	  
	  printf("count of connection: %i\n",sock_count);
	}
	     // printf("receive message:%s\n",buf);
	}
	
	
       }
  }
//	    close(sock);
 pthread_exit(0);

}
int DFA_E(int state,char buf[1024])
{

	     switch (state) {
        case 0:
		{
		if(strcmp(buf,"start")==0)
		state=1;
		if( strcmp(buf,"inc")==0)
		state=1;
		break;
	//	printf("rez cmp: %i",strcmp(buf,"inc"));
		}
        case 1:
		{
		if( strcmp(buf,"inc")==0)
		state=3;
		if( strcmp(buf,"dec")==0)
		state=2;
		break;
		}
        case 2:
		{
		if( strcmp(buf,"inc")==0)
		state=4;
		break;
		}
        case 3:
		{
		if( strcmp(buf,"dec")==0)
		state=2;
		break;
		}
        case 4:
		{
		if( strcmp(buf,"inc")==0)
		state=3;
		if( strcmp(buf,"dec")==0)
		state=1;
		if( strcmp(buf,"close")==0)
		state=5;
		break;
		}
       // default:

        }
return state;


}
