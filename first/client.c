#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int sockfd;
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(address.sin_addr));
    address.sin_port = htons(1234);


    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    connect(sockfd, (struct sockaddr *) &address, sizeof(address));

    char buffer[1024] = {0};
    read(0, buffer, 1024);
    send(sockfd, buffer, strlen(buffer), 0);
    memset(buffer, 0, strlen(buffer));
    recv(sockfd, buffer, 1024, 0);
    printf("received from server: %s\n", buffer);
    return 0;
}