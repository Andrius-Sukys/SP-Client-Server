#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_HOST "127.0.0.1"
#define BACKLOG 10

int c_getaddrinfo(char* port, struct addrinfo* hints, struct addrinfo** res)
{
    return getaddrinfo(DEFAULT_HOST, port, hints, res);
}

int c_socket(int family, int type, int protocol)
{
    return socket(family, type, protocol);
}

int c_connect(int sockfd, struct sockaddr* addr, int addrlen)
{
    return connect(sockfd, addr, addrlen);
}

void c_print(struct addrinfo* ptr, char* port)
{
    printf("RESULT\n");
    printf("Address family: %d\n", ptr->ai_family);
    printf("Socket type: %d\n", ptr->ai_socktype);
    printf("Protocol: %d\n", ptr->ai_protocol);
    printf("Canonical name: %s\n", ptr->ai_canonname);
    printf("Socket address: [%s]:%s\n\n", DEFAULT_HOST, port);
}

int c_validate_port(char* port)
{
    if((int) strtol(port, (char **)NULL, 10) <= 1024)
        return 0;
    else
        return 1;
}

int main(int argc, char* argv[])
{
    if((argc != 3) || ((strcmp(argv[1], "c1")) && (strcmp(argv[1], "c2"))))
    {
        printf("To launch the program type: client (c1/c2) port\n");
        exit(1);
    }

    if((!c_validate_port(argv[2])))
    {
        printf("Port must be higher than 1024!\n");
        exit(1);
    }

    char* port_def = malloc(sizeof(char*) * 6);
    port_def = argv[2];
    int client_type = argv[1][1]-'0';

    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("\nERROR: WSAStartup failed.\n");
        exit(1);
    }

    if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        printf("\nERROR: Version 2.2 of Winsock is not available.\n");
        WSACleanup();
        exit(1);
    }

    printf("\nWSAStartup successful.\n\n");
    
    struct addrinfo hints;
    struct addrinfo* res;
    struct addrinfo* ptr;

    int sockfd;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status;

    status = c_getaddrinfo(port_def, &hints, &res);

    if(status != 0)
    {
        printf("ERROR: getaddrinfo failed.\n");
        WSACleanup();
        exit(1);
    }

    for(ptr = res; ptr != NULL; ptr = ptr->ai_next)
    {
        status = c_socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        sockfd = status;

        if(status == -1)
        {
            printf("ERROR: socket failed.\n");
            continue;
        }

        status = c_connect(sockfd, ptr->ai_addr, ptr->ai_addrlen);

        if(status == -1)
        {
            printf("ERROR: connect failed.\n");
            closesocket(sockfd);
            continue;
        }

        c_print(ptr, port_def);

        break;
    }

    freeaddrinfo(res);
    
    free(port_def);

    if(ptr == NULL)
    {
        printf("An error occurred. Unable to access the socket.\n");
        WSACleanup();
        exit(1);
    }

    printf("Connected.\n\n");

    if(client_type == 1)
    {
        char* msg_send = malloc(sizeof(char) * 1024);
        int bytes_send;

        printf("Enter a line to send:\n");
        scanf("%[^\n]%*c", msg_send);
        bytes_send = strlen(msg_send);

        status = send(sockfd, msg_send, bytes_send, 0);

        if(status == -1)
        {
            printf("ERROR: send failed.\n");
            WSACleanup();
            exit(1);
        }

        printf("\n");

        printf("Sent message: %s\n", msg_send);

        free(msg_send);
    }

    if(client_type == 2)
    {
        char* msg_recv = malloc(sizeof(char) * 1024);
        int bytes_recv;

        printf("Waiting for the message from Server 2.\n\n");

        status = recv(sockfd, msg_recv, 1024, 0);

        if(status == -1)
        {
            printf("ERROR: send failed.\n");
            WSACleanup();
            exit(1);
        }

        bytes_recv = status;

        msg_recv[bytes_recv] = '\0';

        printf("Received message: %s\n", msg_recv);

        free(msg_recv);
    }

    closesocket(sockfd);

    WSACleanup();

    return 0;
}