// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 6000
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // change if server is on another machine

    printf("Guess the number (1-100):\n");

    while (1) {
        printf("Enter guess: ");
        fgets(buffer, MAXLINE, stdin);

        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&servaddr, len);

        int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        printf("Server says: %s\n", buffer);

        if (strcmp(buffer, "Correct!") == 0) {
            break;
        }
    }

    close(sockfd);
    return 0;
}
