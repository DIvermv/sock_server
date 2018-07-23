#ifndef SOCK_SERVER_H
#define SOCK_SERVER_H
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
//#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <sys/epoll.h>
//#include <fcntl.h>
//#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>// получить параметры дискриптора
#include <net/if.h>
int server(int port);
#endif
