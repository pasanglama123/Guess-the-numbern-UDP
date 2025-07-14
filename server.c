// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 6000
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    srand(time(NULL));
    int secret_number = rand() % 100 + 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(1);
    }

    printf("Server started. Secret number is %d\n", secret_number);

    while (1) {
        int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        int guess = atoi(buffer);
        printf("Client guessed: %d\n", guess);

        char *response;
        if (guess < secret_number) {
            response = "Higher";
        } else if (guess > secret_number) {
            response = "Lower";
        } else {
            response = "Correct!";
        }

        sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&cliaddr, len);

        if (strcmp(response, "Correct!") == 0) {
            secret_number = rand() % 100 + 1; // reset game
            printf("New secret number is %d\n", secret_number);
        }
    }

    close(sockfd);
    return 0;
}
