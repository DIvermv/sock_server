#ifndef SOCK_SERVER_H
#define SOCK_SERVER_H
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <unistd.h>
int server(int port);
void *  TCP_10_th_DFA(void * f_data);
#endif
