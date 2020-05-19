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
    memset(&my_addr, 0, sizeof(my_addr)); //数据初始化--清零
	my_addr.sin_family = AF_INET; //设置为IP通信
	my_addr.sin_addr.s_addr = INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上
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
	len = send(client_fd, "Connection Setup!\n", 18, 0);//发送欢迎信息
	
	/*接收客户端的数据并将其发送给客户端--recv返回接收到的字节数，send返回发送的字节数*/
	while (1) {
		printf("Input the message:\n");
		fgets(buffer, BUFFER_SIZE, stdin);
		if(!strcmp(buffer, "quit")) {
			break;
		}
		len = send(client_fd, buffer, strlen(buffer), 0);
		len = recv(client_fd, buffer, BUFFER_SIZE, 0);
		buffer[len] = '\0';
		printf("Receive from client:\n%s\n", buffer);
	}
}