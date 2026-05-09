#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *results;
    struct addrinfo *current;
    int status;
    char ipstr[INET6_ADDRSTRLEN];

    if (argc != 2) //check for a hostname
    {
        fprintf(stderr, "usage: %s hostname\n", argv[0]);
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets

    status = getaddrinfo(argv[1], NULL, &hints, &results);// look up adress of hostname
    if (status != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n\n", argv[1]);

    for (current = results; current != NULL; current = current->ai_next) //loop through the linked list of results
    {
        void *addr;
        char *version;

        if (current->ai_family == AF_INET)
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)current->ai_addr;
            addr = &(ipv4->sin_addr);
            version = "IPv4";
        }
        else
        {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)current->ai_addr;
            addr = &(ipv6->sin6_addr);
            version = "IPv6:";
        }

        inet_ntop(current->ai_family, addr, ipstr, sizeof(ipstr));
        printf("%s: %s\n", version, ipstr);
    }

    freeaddrinfo(results); // free the linked list

    return 0;
}