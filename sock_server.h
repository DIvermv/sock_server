#ifndef FUNC_H
#define FUNC_H
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
int TCP_server(int port);
int UDP_server(int port);
int UDP_con_server(int port);
#endif
