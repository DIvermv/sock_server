all:
	gcc main.c server_proc.c server_th.c -o Sock_server -lpthread
