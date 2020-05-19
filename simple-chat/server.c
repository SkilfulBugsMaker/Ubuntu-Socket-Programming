#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(void) {
    int server_fd;
    int client_fd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
    int len;
    memset(&my_addr, 0, sizeof(my_addr)); 
	my_addr.sin_family = AF_INET; 
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port = htons(8000);
    if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {  
		printf("Socket error!\n");
		return 1;
	}
    if(bind(server_fd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) < 0) {
		printf("Bind error!\n");
		return 1;
	}
    if(listen(server_fd, 5) < 0)
	{
		printf("Listen error!\n");
		return 1;
	}
    int sin_size=sizeof(struct sockaddr_in);
	
	if((client_fd = accept(server_fd, (struct sockaddr *) &remote_addr, &sin_size)) < 0) {
		printf("Accept error!\n");
		return 1;
	}
    fcntl(client_fd, F_SETFL, O_NONBLOCK); // set non blocking
	printf("accept client %s\n",inet_ntoa(remote_addr.sin_addr));
	len = send(client_fd, "Connection Setup!\n", 18, 0); // send welcome message
	
	
	while (1) {
		printf("Input the message:\n");
		fgets(buffer, BUFFER_SIZE, stdin);
		if(strcmp(buffer, "quit") == 0) {
			break;
		}
		len = send(client_fd, buffer, strlen(buffer), 0);
		len = recv(client_fd, buffer, BUFFER_SIZE, 0);
		buffer[len] = '\0';
		printf("Receive from client:\n%s\n", buffer);
	}
}