
#include "sock_server.h"

int server(int port)
{

    int sock, lfd;// 0 - значение дискриптора, 1 - состояние DFA
    struct ssock_tcp{
	    int fd[10];
	    int state[10];
    } sock_tcp;    
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
    int sock_count, ready,epfd;
    struct epoll_event ev;
    struct epoll_event evlist[1];
    if((epfd=epoll_create(12))<0)// создаем на 12 дискрипторов
        perror("epol create:");
     ev.data.fd=lfd;
     ev.events=EPOLLIN;
     if (epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev) < 0)
        perror("epol add:");
     ev.data.fd=sock_udp;
     ev.events=EPOLLIN;
     if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock_udp, &ev) < 0)
        perror("epol add:");
     sock_count=2;

    while(1) //

    { 
       if((ready = epoll_wait(epfd, evlist,1, 100))<0)
            perror("select");

        if(evlist[0].data.fd==lfd)// Поступил новый запрос на соединение, используем accept
        {
         addrlen=sizeof(addr);// инициализируем длину адреса
         if((sock = accept(lfd, (struct sockaddr *) &addr, &addrlen))<0)// пришёл запрос на соединение для TCP
                perror("accept");
	 else
		 printf("accept on: %d\n",addr.sin_addr.s_addr);
	 // добавляем в список прослушки
	 ev.data.fd=sock;
         ev.events=EPOLLIN | EPOLLET;
	if(fcntl(sock, F_SETFL, O_NONBLOCK)<0)
		perror("fcntl");
         if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) < 0)
            perror("epol add:");
	 sock_tcp.fd[sock_count-2]=sock;
	 sock_tcp.state[sock_count-2]=0;
	 sock_count++;
         printf("connect new user: %i. All - %i\n",sock,sock_count-2);
	 if(sock_count==12) // набралось 10 абонентов
	 {
        	pthread_t TCP_10_tid; // идентификатор потока копирования
                pthread_attr_t TCP_10_attr; // атрибуты потока копирования
                pthread_attr_init(&TCP_10_attr);
                  for(int i=0;i<10;i++)
                   if (epoll_ctl(epfd, EPOLL_CTL_DEL, sock_tcp.fd[i], &ev) < 0)
                      perror("epol delete:");
                pthread_create(&TCP_10_tid,&TCP_10_attr,TCP_10_th_DFA,&sock_tcp);// создаем новый поток 
	    sock_count=2; 
	 }
        }
	if( (ready>0) && (evlist[0].data.fd!=lfd) )
	{int cur_sock=-1;
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
             if(( send(evlist[0].data.fd, buf, strlen(buf), 0))<0)
            perror("send");
	    
	if(sock_tcp.state[cur_sock]==5) // надо завершить
	{
          if (epoll_ctl(epfd, EPOLL_CTL_DEL, sock_tcp.fd[cur_sock], &ev) < 0)
             perror("epol delete:");
	  close(sock_tcp.fd[cur_sock]);
	  printf("state %i in %i, close\n",sock_tcp.state[cur_sock],sock_tcp.fd[cur_sock]);
	  for(int i=cur_sock;i<sock_count-1;i++)// удаляем текущий сокет из массива
	  {
		  sock_tcp.fd[i]=sock_tcp.fd[i+1];
		  sock_tcp.state[i]=sock_tcp.state[i+1];
		  }
	  sock_count-=1;
	  
	  printf("count of connection: %i\n",sock_count-2);
	}
	     // printf("receive message:%s\n",buf);
	}
	
	}

        if(evlist[0].data.fd==sock_udp)// поступило сообщение UDP
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
  close(sock_udp);  
return 0;    	
}

