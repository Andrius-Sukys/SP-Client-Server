#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#define DEFAULT_HOST "127.0.0.1"
#define BACKLOG 10

int s_getaddrinfo(char* port, struct addrinfo* hints, struct addrinfo** res)
{
    return getaddrinfo(DEFAULT_HOST, port, hints, res);
}

int s_socket(int family, int type, int protocol)
{
    return socket(family, type, protocol);
}

int s_connect(int sockfd, struct sockaddr* addr, int addrlen)
{
    return connect(sockfd, addr, addrlen);
}

int s_bind(int sockfd, struct sockaddr* addr, int addrlen)
{
    return bind(sockfd, addr, addrlen);
}

int s_listen(int sockfd)
{
    return listen(sockfd, BACKLOG);
}

int s_accept(int sockfd, struct sockaddr* their_addr, socklen_t* their_addrlen)
{
    return accept(sockfd, their_addr, their_addrlen);
}

void s_print(struct addrinfo* ptr, char* port)
{
    printf("RESULT\n");
    printf("Address family: %d\n", ptr->ai_family);
    printf("Socket type: %d\n", ptr->ai_socktype);
    printf("Protocol: %d\n", ptr->ai_protocol);
    printf("Canonical name: %s\n", ptr->ai_canonname);
    printf("Socket address: [%s]:%s\n\n", DEFAULT_HOST, port);
}

int s_validate_port(char* port)
{
    if((int) strtol(port, (char **)NULL, 10) <= 1024)
        return 0;
    else
        return 1;
}

int main(int argc, char* argv[])
{
    if((argc != 3) || ((strcmp(argv[1], "s1")) && (strcmp(argv[1], "s2"))))
    {
        printf("To launch the program type: server (s1/s2) port\n");
        exit(1);
    }

    if((!s_validate_port(argv[2])))
    {
        printf("Port must be higher than 1024!\n");
        exit(1);
    }

    char* port_def = malloc(sizeof(char*) * 6);
    port_def = argv[2];
    int server_type = argv[1][1]-'0';

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

    status = s_getaddrinfo(port_def, &hints, &res);

    if(status != 0)
    {
        printf("ERROR: getaddrinfo failed.\n");
        WSACleanup();
        exit(1);
    }

    for(ptr = res; ptr != NULL; ptr = ptr->ai_next)
    {
        status = s_socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        sockfd = status;

        if(status == -1)
        {
            printf("ERROR: socket failed.\n");
            continue;
        }

        if(server_type == 1)
        {
            status = s_bind(sockfd, ptr->ai_addr, ptr->ai_addrlen);

            if(status == -1)
            {
                printf("ERROR: bind failed.\n");
                closesocket(sockfd);
                continue;
            }

            s_print(ptr, port_def);
        }
        
        if(server_type == 2)
        {
            status = s_connect(sockfd, ptr->ai_addr, ptr->ai_addrlen);

            if(status == -1)
            {
                printf("ERROR: connect failed.\n");
                closesocket(sockfd);
                continue;
            }
        }

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

    if(server_type == 1)
    {
        status = s_listen(sockfd);

        printf("Waiting for Client 1. Listening...\n");

        if(status == -1)
        {
            printf("ERROR: listen failed.\n");
            closesocket(sockfd);
            WSACleanup();
            exit(1);
        }
    }

    int sockfd2;
    struct sockaddr_storage addr2;
    socklen_t addr2_size;

    addr2_size = sizeof addr2;

    if(server_type == 1)
    {
        status = s_accept(sockfd, (struct sockaddr *)&addr2, &addr2_size);

        sockfd2 = status;

        if(status == -1)
        {
            printf("ERROR: accept failed.\n");
            WSACleanup();
            exit(1);
        }

        printf("Connection with Client 1 established.\n\n");
    }

    struct addrinfo hints3;
    struct addrinfo* res3;
    struct addrinfo* ptr3;

    int sockfd3;

    memset(&hints3, 0, sizeof hints3);

    hints3.ai_family = AF_UNSPEC;
    hints3.ai_socktype = SOCK_STREAM;

    char* port_new = malloc(sizeof(char) * 6);

    status = -1;

    do
    {
        printf("Enter a new port (>1024): ");
        scanf("%[^\n]%*c", port_new);
        printf("\n");
        if(s_validate_port(port_new))
            status = 0;
    } while (status == -1);

    status = s_getaddrinfo(port_new, &hints3, &res3);

    if(status != 0)
    {
        printf("ERROR: getaddrinfo failed.\n");
        WSACleanup();
        exit(1);
    }

    for(ptr3 = res3; ptr3 != NULL; ptr3 = ptr3->ai_next)
    {
        status = s_socket(ptr3->ai_family, ptr3->ai_socktype, ptr3->ai_protocol);

        sockfd3 = status;

        if(status == -1)
        {
            printf("ERROR: socket failed.\n");
            continue;
        }

        status = s_bind(sockfd3, ptr3->ai_addr, ptr3->ai_addrlen);

        if(status == -1)
        {
            printf("ERROR: bind failed.\n");
            closesocket(sockfd3);
            continue;
        }

        s_print(ptr3, port_new);

        break;
    }

    freeaddrinfo(res3);

    free(port_new);

    if(ptr3 == NULL)
    {
        printf("An error occurred. Unable to access the socket.\n");
        WSACleanup();
        exit(1);
    }

    status = s_listen(sockfd3);

    if(server_type == 1)
        printf("Waiting for Server 2. Listening...\n");
    if(server_type == 2)
        printf("Waiting for Client 2. Listening...\n");

    if(status == -1)
    {
        printf("ERROR: listen failed.\n");
        closesocket(sockfd3);
        WSACleanup();
        exit(1);
    }

    int sockfd4;
    struct sockaddr_storage addr4;
    socklen_t addr4_size;

    addr4_size = sizeof addr4;
    status = s_accept(sockfd3, (struct sockaddr *)&addr4, &addr4_size);

    sockfd4 = status;

    if(status == -1)
    {
        printf("ERROR: accept failed.\n");
        WSACleanup();
        exit(1);
    }

    if(server_type == 1)
        printf("Connection with Server 2 established.\n");
    if(server_type == 2)
        printf("Connection with Client 2 established.\n");

    printf("\n");
    
    char* msg_recv = malloc(sizeof(char) * 1024);
    int bytes_recv;

    if(server_type == 1)
    {
        printf("Waiting for the message from Client 1.\n\n");
        status = recv(sockfd2, msg_recv, 1024, 0);
    }
    if(server_type == 2)
    {
        printf("Waiting for the message from Server 1.\n\n");
        status = recv(sockfd, msg_recv, 1024, 0);
    }
        
    
    if(status == -1)
    {
        printf("ERROR: recv failed.\n");
        WSACleanup();
        exit(1);
    }

    bytes_recv = status;

    msg_recv[bytes_recv] = '\0';

    printf("Received message: %s\n", msg_recv);

    if(server_type == 1)
        status = send(sockfd4, msg_recv, bytes_recv, 0);
    if(server_type == 2)
        status = send(sockfd4, msg_recv, bytes_recv, 0);

    if(status == -1)
    {
        printf("ERROR: send failed.\n");
        WSACleanup();
        exit(1);
    }

    printf("Sent message: %s\n", msg_recv);

    free(msg_recv);

    closesocket(sockfd);
    closesocket(sockfd2);
    closesocket(sockfd3);
    closesocket(sockfd4);

    WSACleanup();

    return 0;
}