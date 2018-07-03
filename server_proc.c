
#include "sock_server.h"

int TCP_server(int port)
{

    int sock, lfd;
    struct sockaddr_in addr;
    socklen_t addrlen;

    lfd = socket(AF_INET, SOCK_STREAM, 0);// TCP
    if(lfd < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
    pthread_t TCP_tid; // идентификатор потока копирования
    pthread_attr_t TCP_attr; // атрибуты потока копирования
    pthread_attr_init(&TCP_attr);
    pthread_create(&TCP_tid,&TCP_attr,TCP_th,&sock);// создаем новый поток
    }
  close(lfd);  
return 0;    	
}


int UDP_server(int port)
{
    struct _f_data
    {
	    int sock;
            struct sockaddr_in addr;
            char buf[1024];
    } f_data;
    socklen_t addrlen;
    int bytes_read;

    f_data.sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(f_data.sock < 0)
    {
        perror("socket");
        exit(1);
    }
    
    f_data.addr.sin_family = AF_INET;
    f_data.addr.sin_port = htons(port);
    f_data.addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(f_data.sock, (struct sockaddr *)&f_data.addr, sizeof(f_data.addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    while(1)
    {
	    addrlen=sizeof(f_data.addr);
        bytes_read = recvfrom(f_data.sock, f_data.buf, 1024, 0, (struct sockaddr *) &f_data.addr, &addrlen);
        f_data.buf[bytes_read] = '\0';
    pthread_t UDP_tid; // идентификатор потока копирования
    pthread_attr_t UDP_attr; // атрибуты потока копирования
    pthread_attr_init(&UDP_attr);
    pthread_create(&UDP_tid,&UDP_attr,UDP_th,&f_data);// создаем новый поток
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
