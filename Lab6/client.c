#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
//https://www.geeksforgeeks.org/computer-networks/simple-client-server-application-in-c/
//used a bit of ai to help with the file transfer mode, but now I understand how it works
#define PORT 9001
#define BUFFER_SIZE 100
#define FILE_BUFFER_SIZE 64

int main(int argc, char *argv[])
{
    int sockD;
    struct sockaddr_in servAddr;

    if (argc != 2)
    {
        printf("Usage: %s <mode>\n", argv[0]);
        printf("Mode 1 = echo mode\n");
        printf("Mode 2 = file transfer mode\n");
        return 1;
    }

    char *mode = argv[1];

    // Create client socket
    sockD = socket(AF_INET, SOCK_STREAM, 0);

    if (sockD == -1)
    {
        perror("socket");
        return 1;
    }

    // Define server address
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockD, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
    {
        perror("connect");
        return 1;
    }

    printf("Connected to server.\n");

    // Send mode first
    send(sockD, mode, strlen(mode), 0);

    // Echo mode
    if (strcmp(mode, "1") == 0)
    {
        char buffer[BUFFER_SIZE + 1];
        char response[BUFFER_SIZE + 1];

        printf("Echo mode. Type close to quit.\n");

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            memset(response, 0, sizeof(response));

            printf("Enter message: ");
            fgets(buffer, sizeof(buffer), stdin);

            buffer[strcspn(buffer, "\n")] = '\0';

            send(sockD, buffer, strlen(buffer), 0);

            int bytesReceived = recv(sockD, response, BUFFER_SIZE, 0);

            if (bytesReceived <= 0)
            {
                printf("Server closed connection.\n");
                break;
            }

            response[bytesReceived] = '\0';

            printf("Server replied: %s\n", response);

            if (strcmp(buffer, "close") == 0)
            {
                break;
            }
        }
    }

    // File transfer mode
    else if (strcmp(mode, "2") == 0)
    {
        printf("Receiving file from server...\n");

        FILE *file = fopen("received_file.txt", "wb");

        if (file == NULL)
        {
            perror("fopen");
            close(sockD);
            return 1;
        }

        char fileBuffer[FILE_BUFFER_SIZE];
        int bytesReceived;

        while ((bytesReceived = recv(sockD, fileBuffer, FILE_BUFFER_SIZE, 0)) > 0)
        {
            fwrite(fileBuffer, 1, bytesReceived, file);
        }

        fclose(file);

        printf("File received and saved as received_file.txt\n");
    }

    else
    {
        printf("Invalid mode. Use 1 or 2.\n");
    }

    close(sockD);

    return 0;
}