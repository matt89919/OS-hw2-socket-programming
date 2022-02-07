#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>

#include "sock.h"


int main(int argc, char **argv)
{
    int opt;
    char *server_host_name = NULL, *server_port = NULL;


    /* Parsing args */
    while ((opt = getopt(argc, argv, "h:p:")) != -1)
    {
        switch (opt)
        {
        case 'h':
            server_host_name = malloc(strlen(optarg) + 1);
            strncpy(server_host_name, optarg, strlen(optarg));
            break;
        case 'p':
            server_port = malloc(strlen(optarg) + 1);
            strncpy(server_port, optarg, strlen(optarg));
            break;
        case '?':
            fprintf(stderr, "Unknown option \"-%c\"\n", isprint(optopt) ?
                    optopt : '#');
            return 0;
        }
    }

    if (!server_host_name)
    {
        fprintf(stderr, "Error!, No host name provided!\n");
        exit(1);
    }

    if (!server_port)
    {
        fprintf(stderr, "Error!, No port number provided!\n");
        exit(1);
    }

    /* Open a client socket fd */
    int clientfd __attribute__((unused)) = open_clientfd(server_host_name, server_port);

    /* Start your coding client code here! */

    char cmd[1024],buff[1024];

    printf("client %d connected to port %s\n",clientfd,server_port);
    printf("please enter command :\n");
    printf("SET [KEY] [VALUE]\n");
    printf("GET [KEY]\n");
    printf("DELETE [KEY]\n\n\n");



    while(1)
    {

        fgets(cmd,1024,stdin);
        //print the result of the send
        if(send(clientfd, cmd, 1024, 0) < 0)
            printf("send failed");
        if(recv(clientfd, buff, 1024, 0) < 0)
            printf("server didnt reply");
        else
            printf("%s\n",buff);

    }

    close(clientfd);

    return 0;
}
