
#include "sock_server.h"
void *  TCP_th_DFA(void * f_data)
{
int sock =* (int*) f_data;
int state = 0;// инициалировали в начало графа
    char buf[1024];
    int bytes_read;
	      printf("Start DFA:%s\n",buf);
	while(state!=5)
             {
             if(( bytes_read = recv(sock, buf, 1024, 0))<0)
            perror("receive");
	      printf("receive message:%s\n",buf);
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
	      printf("state :%i\n",state);
	      sprintf(buf,"%i\n",state);
             if(( send(sock, buf, strlen(buf), 0))<0)
            perror("send");
	    sleep(1);
              }
	    close(sock);
 pthread_exit(0);

}
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
void *  UDP_th(void * F_data)
{
    char  mes[1540];
    struct _f_data
    {
	    int sock;
            struct sockaddr_in addr;
            char buf[1024];
    };
   struct _f_data f_data =* (struct _f_data*) F_data;

        printf("receive:%s\n",f_data.buf);

	sprintf(mes,"%s return\n",f_data.buf);
         sendto(f_data.sock, mes, strlen(mes), 0, (struct sockaddr *)&f_data.addr, sizeof(f_data.addr));	
	   // close(f_data.sock);
 pthread_exit(0);
}
