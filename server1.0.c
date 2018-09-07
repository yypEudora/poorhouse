#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


#define _PORT_ 9999
#define backlot 10

int main()
{
	int sock=socket(AF_INET, SOCK_STREAM, 0);
	if(sock==-1)
	{
		perror("sock");
		exit(1);
	}

	struct sockaddr_in server_socket;
	struct sockaddr_in client_socket;

	server_socket.sin_family=AF_INET;
	server_socket.sin_port=htons(_PORT_);
	server_socket.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if(bind(sock, (struct sockaddr*)&server_socket, sizeof(struct sockaddr_in))==-1)
	{
		perror("bind");
		close(sock);
		exit(2);
	}
	
	if(listen(sock, backlot)==-1)
	{
		perror("listen");
		close(sock);
		exit(3);
	}

	printf("bind and listen successfully.\n");
	
	while(1)
	{
		socklen_t len=0;
		int client_sock=accept(sock, (struct sockaddr*)&client_socket, &len);
		if(client_sock<0)
		{
			perror("accept");
			close(sock);
			exit(4);
		}

		char buf_client_ip[INET_ADDRSTRLEN];		
		memset(buf_client_ip, '\0', sizeof(buf_client_ip));
		inet_ntop(AF_INET, &client_socket.sin_addr, buf_client_ip, sizeof(buf_client_ip));
		printf("connet successfully, ip is %s, port is %d\n", buf_client_ip, ntohs(client_socket.sin_port));
		
		while(1)
		{
			char buf[1024];
			memset(buf, '\0', sizeof(buf));
			read(client_sock, buf, sizeof(buf));
			printf("client@:%s\n", buf);
			printf("server@:");
			
			memset(buf, '\0', sizeof(buf));
			fgets(buf, sizeof(buf), stdin);
			buf[strlen(buf)-1]='\0';
			write(client_sock, buf, strlen(buf)+1);
			printf("----wait----");
		}
	}
	close(sock);
	return 0;
}
