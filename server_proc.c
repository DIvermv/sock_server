
#include "sock_server.h"

int TCP_server(int port)
{

    int sock, lfd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char buf[1024];
    int bytes_read;

    lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
   // addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);// замыкаем на себя (127.0.0.1)
    if(bind(lfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(lfd, 1);
    printf("Incoming connect ready\n");
    while(1)
    {
        sock = accept(lfd, (struct sockaddr *) &addr, &addrlen);// пришло сообщение
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

            bytes_read = recv(sock, buf, 1024, 0);
        while(bytes_read > 0)
        {
	    printf("receive message:%s",buf);
            send(sock, buf, bytes_read, 0);
            bytes_read = recv(sock, buf, 1524, 0);
        }
    
        close(sock);
    }
return 0;    	
}


int UDP_server(int port)
{
    int sock;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char buf[1024];
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
        bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &addrlen);
        buf[bytes_read] = '\0';
        printf("receive: %s",buf);
    }

return 0;    	
}
