
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    int sockfd, clientfd;
    // struct sockaddr_in address;
    struct sockaddr_in clientAddress;
    int addrsize = sizeof(struct sockaddr_storage);
    int yes = 1;
    struct addrinfo hints, *res;

    // address.sin_addr.s_addr = INADDR_ANY;
    // address.sin_family = AF_INET;
    // address.sin_port = htons(1234);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "1234", &hints, &res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // int sockfd2;
    // sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    printf("Server socket: %d\n", sockfd);
    // printf("Server 2 socket: %d\n", sockfd2);

    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, 1);

    clientfd = accept(sockfd, (struct sockaddr *) &clientAddress, (socklen_t *) &addrsize);
    printf("client fd: %d\n", clientfd);

    char buffer[1024] = {0};
    recv(clientfd, buffer, 1024, 0);
    printf("Received from client: %s\n", buffer);

    char newBuffer[1024] = {0};
    sprintf(newBuffer, "From server: %s", buffer);
    send(clientfd, newBuffer, strlen(newBuffer), 0);

    // while(1)
    // {

    // }
    return 0;
}