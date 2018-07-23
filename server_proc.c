
#include "sock_server.h"

int server(int port)
{

    int sock_udp;
    struct sockaddr_in addr;
    socklen_t addrlen;
    int val=1;

    if((sock_udp = socket(AF_INET, SOCK_DGRAM, 0))<0)// UDP
        perror("socket UDP");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr("192.168.0.255");
    setsockopt(sock_udp,SOL_SOCKET,SO_BROADCAST,&val,sizeof(val));
    /*if(bind(sock_udp, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        perror("bind UDP");
    else
	 printf("bind UDP on port: %i\n",port+1);   */
   for(int i=0;i<20;i++)
   {
	char mes[1030];
	sprintf(mes,"Broadcast message # %i\n",i);
         sendto(sock_udp, mes, strlen(mes), 0, (struct sockaddr *)&addr, sizeof(addr));	
	perror("send:");
   }
    
  close(sock_udp);  
return 0;    	
}

