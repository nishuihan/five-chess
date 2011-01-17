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

#include <unistd.h>
#include <fcntl.h>

#include <sys/select.h>

#include "main.h"


//#define SERVER_PORT 20001
#define SERVER_PORT 31313 
message_t buffer;
int fd;

//int main(int argc, const char *argv[])
int client_op(void)
{
    int client_socket;
    char client_ip[] = "10.1.14.41";
    //char client_ip[] = "10.1.14.68";
    struct sockaddr_in server;
    //char flag_s = 0;
    socklen_t server_len;
    fd_set input_fd;

    fd = open("/dev/input/mice", O_RDWR|O_NONBLOCK);
    if(fd < 0)
    {
        perror("open");
        exit(1);
    }

    if((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    else
        printf("socket success! \n");

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(client_ip);
    //server.sin_addr.s_addr = client_ip;
    flag = 0;
    #if 0
    buffer.x = 0;
    buffer.y = 0;
    server_len = sizeof(server);
    sendto(client_socket, &buffer, sizeof(buffer), 0, (struct sockaddr *)&server, server_len);
    #endif
    while(1)
    {
        FD_ZERO(&input_fd);
        FD_SET(fd, &input_fd);
        FD_SET(client_socket, &input_fd);

        if((select(client_socket+ 1, &input_fd, NULL, NULL, NULL)) < 0)
        {
            perror("select");
            continue;
        }
        if(FD_ISSET(fd, &input_fd))
        {

            global_color = 0x00ffffff;
            who = 1;
            mouse_doing();
            if(flag == 1)
            {
                //global_color = 0x00ffffff;
                    //who = 1;
                flag = 0;
                //mouse_doing();
                buffer.x = global_x;
                buffer.y = global_y;
                printf("%d  %d\n", global_x, global_y);
                server_len = sizeof(server);
                sendto(client_socket, &buffer, sizeof(buffer), 0, (struct sockaddr *)&server, server_len);
                //flag_s = 1;
            }
        }
        if(FD_ISSET(client_socket, &input_fd))
        {
            printf("you re\n");
            server_len = sizeof(server);
            recvfrom(client_socket, &buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &server_len);
            global_x = buffer.x;
            global_y = buffer.y;
            global_color = 0x00000000;
            who = 2;

            scan_point(STARTX + global_x*SPACE, STARTY + global_y*SPACE);
            //flag_s = 0;
            board[global_x + global_y*H_NUM] = who;
            check_all();
        }
       

    }
    close(client_socket);
    return 0;
}
