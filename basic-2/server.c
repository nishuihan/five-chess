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

#include "main.h"

#define SERVER_PORT 31313 
int fd;


message_t buffer;

//int main(int argc, const char *argv[])
int  server_op(void)
{
    int  len;
    int server_sock;
    socklen_t client_len;
    struct sockaddr_in server, client;
    struct in_addr in;
    char flag_s = 1;

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
        if(flag_s == 1)
        {
            client_len = sizeof(client);
            len = recvfrom(server_sock, &buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &client_len);
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
                global_x = buffer.x;
                global_y = buffer.y;
                    //scan_point(global_x + STARTX, global_y + STARTY);
                scan_point(STARTX + global_x*SPACE, STARTY + global_y*SPACE);
                flag_s = 0;
            }
        }

        mouse_doing();

        if(flag == 1)
        {
            flag = 0;
            buffer.x = global_x;
            buffer.y = global_y;
            //client_len = sizeof(client);
            sendto(server_sock, &buffer, sizeof(buffer), 0, (struct sockaddr *)&client, client_len);
            flag_s = 1;
        }
    }
    printf("client close the connect, please wait for new connect\n");
    close(server_sock);
    return 0;
}
