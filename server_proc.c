
#include "sock_server.h"

int TCP_server(int port)
{

    int sock, lfd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char buf[1024];
    int bytes_read;

    lfd = socket(AF_INET, SOCK_STREAM, 0);// TCP
   // lfd = socket(AF_INET,  SOCK_DGRAM, 0);//UDP присоединенный сокет
    if(lfd < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
 //  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// замыкаем на себя (127.0.0.1)
    if(bind(lfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(lfd, 1);
        perror("listen");
    while(1) //цикл ожидания подключения
    { 
         addrlen=sizeof(addr);// инициализируем длину адреса
       sock = accept(lfd, (struct sockaddr *) &addr, &addrlen);// пришло сообщение для TCP
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }
	if(fork()==0) // потомок
	{
            close(lfd);
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
	    return 1;
	}
	else// родительский процесс
	{
	close(sock);
	}
    }
  close(lfd);  
return 0;    	
}


int UDP_server(int port)
{
    int sock;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char buf[1024];
    char  mes[]="recieve message\n";
    int bytes_read;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    while(1)
    {
	    addrlen=sizeof(addr);
	    
        bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
	if(fork()==0)// обработчик
	{
        buf[bytes_read] = '\0';
        printf("receive:%s  %i\n",buf,bytes_read);
	sprintf(mes,"%s return\n",buf);
         sendto(sock, mes, strlen(mes), 0, (struct sockaddr *)&addr, sizeof(addr));	
	sleep(1);
	close(sock);
	    return 1;
	}
//	else
//	close(sock);
    }

return 0;    	
}
//-----------------------------------------
int UDP_con_server(int port)
{
    int sock,sock_out;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char buf[1024];
    char  mes[]="recieve message\n";
    int bytes_read;

    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    bytes_read = recv(sock, buf, 1024, 0);// прочитали один раз
        perror("recv1");

    sock_out= socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_port = htons(port+1);
    if(connect(sock_out, (struct sockaddr *)&addr, sizeof(addr)) < 0) // подсоединяемся к серверу
     {
         perror("connect");
         exit(2);
     }
    send(sock_out, buf, strlen(buf), 0);	
      //   perror("send1");
    while(1)
    {
        bytes_read = recv(sock, buf, 1024, 0);
       // perror("recv");
        buf[bytes_read] = '\0';
        printf("receive:%i bytes.  %s\n",bytes_read,buf);
	if(bytes_read>0)
	{
         send(sock_out, buf, strlen(buf), 0);	
       // perror("send");
	}
//	sleep(1);
    }

return 0;    	
}
