
#include "sock_server.h"

int server(int port)
{

    int sock_udp;
    struct sockaddr_in addr;
    socklen_t addrlen;
    int val=1;

    if((sock_udp = socket(AF_INET, SOCK_DGRAM, 0))<0)// UDP
        perror("socket UDP");


    struct ifreq if_buff;
    memset(&if_buff, 0, sizeof(if_buff));
    strncpy(if_buff.ifr_name, "wlp3s0", sizeof("wlp3s0"));
   // ioctl(sock_udp, SIOCGIFADDR, &if_buff);// имя интерфейса
    if(ioctl(sock_udp, SIOCGIFBRDADDR, &if_buff)<0)// широковещательный адрес
    perror("ioctl");
   // addr =* (struct sockaddr_in *) &if_buff.ifr_addr;
    memcpy(&addr,&if_buff.ifr_addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    printf("Broadcast IP:  %s\n",inet_ntoa(addr.sin_addr));
    setsockopt(sock_udp,SOL_SOCKET,SO_BROADCAST,&val,sizeof(int));
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

