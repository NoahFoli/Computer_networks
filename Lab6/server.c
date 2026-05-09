#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define PORT 9001
#define BUFFER_SIZE 100
#define FILE_BUFFER_SIZE 64
//https://www.geeksforgeeks.org/computer-networks/simple-client-server-application-in-c/
int main()
{
    int servSockD;
    int clientSocket;
    struct sockaddr_in servAddr;

    // Create server socket
    servSockD = socket(AF_INET, SOCK_STREAM, 0);

    if (servSockD == -1)
    {
        perror("socket");
        return 1;
    }

    // Define server address
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to IP and port
    if (bind(servSockD, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
    {
        perror("bind");
        return 1;
    }

    // Listen for client connections
    if (listen(servSockD, 1) == -1)
    {
        perror("listen");
        return 1;
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept client connection
    clientSocket = accept(servSockD, NULL, NULL);

    if (clientSocket == -1)
    {
        perror("accept");
        return 1;
    }

    printf("Client connected.\n");

    // Receive mode from client
    char mode[10] = {0};

    int bytesReceived = recv(clientSocket, mode, sizeof(mode) - 1, 0);

    if (bytesReceived <= 0)
    {
        printf("Client disconnected before sending mode.\n");
        close(clientSocket);
        close(servSockD);
        return 0;
    }

    mode[bytesReceived] = '\0';
    mode[strcspn(mode, "\n")] = '\0';

    printf("Mode received: %s\n", mode);

    // Mode 1: Echo mode
    if (strcmp(mode, "1") == 0)
    {
        printf("Echo mode started.\n");

        char buffer[BUFFER_SIZE + 1];

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));

            bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

            if (bytesReceived <= 0)
            {
                printf("Client disconnected.\n");
                break;
            }

            buffer[bytesReceived] = '\0';
            buffer[strcspn(buffer, "\n")] = '\0';

            printf("Received from client: %s\n", buffer);

            if (strcmp(buffer, "close") == 0)
            {
                char goodbye[] = "goodbye";
                send(clientSocket, goodbye, strlen(goodbye), 0);
                printf("Closing client connection.\n");
                break;
            }
            else
            {
                send(clientSocket, buffer, strlen(buffer), 0);
            }
        }
    }

    // Mode 2: File transfer mode
    else if (strcmp(mode, "2") == 0)
    {
        printf("File transfer mode started.\n");

        FILE *file = fopen("server_file.txt", "rb");

        if (file == NULL)
        {
            perror("fopen");
            close(clientSocket);
            close(servSockD);
            return 1;
        }

        char fileBuffer[FILE_BUFFER_SIZE];
        int bytesRead;

        while ((bytesRead = fread(fileBuffer, 1, FILE_BUFFER_SIZE, file)) > 0)
        {
            send(clientSocket, fileBuffer, bytesRead, 0);
        }

        fclose(file);

        printf("File sent to client.\n");
    }

    else
    {
        printf("Invalid mode.\n");
    }

    close(clientSocket);
    close(servSockD);

    return 0;
}