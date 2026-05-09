#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    int sockfd, clientfd;
    struct sockaddr_in clientAddress;
    int addrsize = sizeof(struct sockaddr_storage);
    int yes = 1;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(NULL, "1234", &hints, &res);

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    printf("Server socket: %d\n", sockfd);

    bind(sockfd, res->ai_addr, res->ai_addrlen);
    // listen(sockfd, 1);

    // clientfd = accept(sockfd, (struct sockaddr *) &clientAddress, (socklen_t *) &addrsize);

    FILE *fin = fopen("test.txt", "rb");
    fseek(fin, 0, SEEK_END);
    ssize_t filesize = ftell(fin);
    rewind(fin);
    printf("file size: %ld\n", filesize);
    // char* filebuf;


    return 0;
}