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
#define BUFFER_SIZE 256

char buffer[BUFFER_SIZE];

int main(int argc, const char *argv[])
{
    int i, n, len;
    int client_socket;
    struct sockaddr_in server;
    socklen_t server_len;

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
    server.sin_addr.s_addr = inet_addr(argv[1]);

    while(1)
    {
        i = write(STDOUT_FILENO, ">  ", 3);
        if((len = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0 )
        {
            server_len = sizeof(server);
            n = sendto(client_socket, buffer, len, 0, (struct sockaddr *)&server, server_len);
        }

        if(buffer[0] == '.')
            break;

        server_len = sizeof(server);
        n = recvfrom(client_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server, &server_len);
        if(n > 0)
        {
            i = write(STDOUT_FILENO, buffer, n);
        }
    }
    close(client_socket);
    return 0;
}
