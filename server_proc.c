
#include "sock_server.h"

int server(int port)
{

    int sock, lfd;
    int sock_udp;
    struct sockaddr_in addr;
    socklen_t addrlen;

    lfd = socket(AF_INET, SOCK_STREAM, 0);// TCP
    if((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)// TCP
        perror("socket TCP");
    if((sock_udp = socket(AF_INET, SOCK_DGRAM, 0))<0)// UDP
        perror("socket UDP");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(lfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        perror("bind TCP");
    else
	 printf("bind TCP on port: %i\n",port);   
    addr.sin_port = htons(port+1);
    if(bind(sock_udp, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        perror("bind UDP");
    else
	 printf("bind UDP on port: %i\n",port+1);   
    if(listen(lfd, 2)<0)
        perror("listen");
   struct pollfd pollFd[32]; 
    int sock_count, ready;
    while(1) //

    { 
     pollFd[0].fd=lfd;
     pollFd[0].events=POLLIN;
     pollFd[1].fd=sock_udp;
     pollFd[1].events=POLLIN;
     sock_count=2;
	// Задаём таймаут
     struct  timeval timeout;
     timeout.tv_sec = 10;// ожидаем 30 секунд
     timeout.tv_usec = 0;
       if((ready = poll(pollFd, sock_count, -1))<0)
            perror("select");

        if(ready==0)// Поступил новый запрос на соединение, используем accept
        {
         addrlen=sizeof(addr);// инициализируем длину адреса
         if((sock = accept(lfd, (struct sockaddr *) &addr, &addrlen))<0)// пришёл запрос на соединение для TCP
                perror("accept");
	 else
		 printf("accept on: %d\n",addr.sin_addr.s_addr);
        }

        if(ready==1)// поступило сообщение UDP
	{
        addrlen=sizeof(addr);
        char buf[1024];
        int bytes_read = recvfrom(sock_udp, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
        buf[bytes_read] = '\0';
        printf("receive UDP:%s from %d\n",buf,addr.sin_addr.s_addr);
	char mes[1030];
	sprintf(mes,"%s return\n",buf);
         sendto(sock_udp, mes, strlen(mes), 0, (struct sockaddr *)&addr, sizeof(addr));	
	}

    }
  close(lfd);  
return 0;    	
}

