#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

double time_diff(struct timeval start, struct timeval end) {
    return (double)(end.tv_sec - start.tv_sec) * 1000000 + (double)(end.tv_usec - start.tv_usec);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <output_file>\n", argv[0]);
        return -1;
    }

    char *output_file = argv[1];
    FILE *fp = fopen(output_file, "w");
    if (!fp) {
        perror("File opening failed");
        return -1;
    }

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // UDP socket
    if (sockfd == -1) {
        perror("Socket creation failed");
        fclose(fp);
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(sockfd);
        fclose(fp);
        return -1;
    }
    printf("Listening on port 8080\n");
    struct timeval t1, t2;
    int packet_number = 0;
    while (1) {
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len);
        printf("Received packet %d\n", buffer);
        packet_number++;
    }

    fclose(fp);
    close(sockfd);
    return 0;
}