#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#define SERVER_PORT 20001
#define BUFFER_SIZE 20 

//int buffer[BUFFER_SIZE];
typedef struct{
    char dest;
    char source;
    char type;
    char len;
    int x;
    int y;
}message_t;
message_t buffer;

int main(int argc, const char *argv[])
{
    int c;
    int i, len;
    int server_sock;
    socklen_t client_len;
    struct sockaddr_in server, client;
    struct in_addr in;

    if((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    else
        printf("socket success!\n");
    
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = ntohs(INADDR_ANY); 

    if(bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind");
        exit(1);
    }
    else
        printf("bind address success!\n");
    
    while(1)
    {
        client_len = sizeof(client);
        len = recvfrom(server_sock, &buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client, &client_len);
        if(len < 0)
        {
            close(server_sock);
            perror("recvform");
            exit(1);
        }
        else
        {
            printf("recvform success !\n");
            in.s_addr = client.sin_addr.s_addr;
            printf("client ip = %s\n", inet_ntoa(in));
            printf("client port = %d\n", ntohs(client.sin_port));
            printf("\n");
        }

            buffer.x =  buffer.x + 1;
            buffer.y = buffer.y + 1;

        sendto(server_sock, &buffer, len, 0, (struct sockaddr *)&client, client_len);
    }
    printf("client close the connect, please wait for new connect\n");
    close(server_sock);
    return 0;
}
