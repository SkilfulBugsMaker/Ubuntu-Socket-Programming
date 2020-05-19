#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <fcntl.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024


int main(int argc, char *argv[]) {
	
	int client_fd;
	struct sockaddr_in remote_addr;
	char buffer[BUFFER_SIZE];
	int len;
	memset(&remote_addr,0,sizeof(remote_addr));
	remote_addr.sin_family = AF_INET;
	remote_addr.sin_addr.s_addr = inet_addr("182.92.87.240"); // set server IP
	remote_addr.sin_port = htons(8000); // set server port
	if((client_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket error!\n");
		return 1;
	}
	if(connect(client_fd, (struct sockaddr *) &remote_addr, sizeof(struct sockaddr)) < 0)
	{
		printf("Connect error!\n");
		return 1;
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK); // set non blocking
	len = recv(client_fd, buffer, BUFFER_SIZE, 0); // receive server message
    buffer[len]='\0';
	printf("%s",buffer);
	while (1) {
		printf("Input the message:\n");
		fgets(buffer, BUFFER_SIZE, stdin);
		if(strcmp(buffer, "quit") == 0) {
			break;
		}
		len = send(client_fd, buffer, strlen(buffer), 0);
		len = recv(client_fd, buffer, BUFFER_SIZE, 0);
		buffer[len] = '\0';
		printf("Receive from server:\n%s\n", buffer);
	}

	close(client_fd);
	return 0;
}
