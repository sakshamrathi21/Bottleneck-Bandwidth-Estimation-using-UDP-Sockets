#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

void send_packet_pairs(int sockfd, struct sockaddr_in dest_addr, int P, int spacing_ms, int total_pairs) {
    for (int i = 0; i < total_pairs; ++i) {
        // Packet 1
        char packet1[P];
        sprintf(packet1, "Packet %d", 2 * i + 1);
        sendto(sockfd, packet1, sizeof(packet1), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        
        // Packet 2
        char packet2[P];
        sprintf(packet2, "Packet %d", 2 * i + 2);
        sendto(sockfd, packet2, sizeof(packet2), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));

        // Wait for the specified interval
        usleep(spacing_ms * 1000);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <packet_size_in_bits> <dest_ip> <spacing_ms> <total_packet_pairs>\n", argv[0]);
        return -1;
    }

    int P = atoi(argv[1]);
    char *dest_ip = argv[2];
    int spacing_ms = atoi(argv[3]);
    int total_pairs = atoi(argv[4]);

    int sockfd;
    struct sockaddr_in dest_addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // UDP socket
    if (sockfd == -1) {
        perror("Socket creation failed");
        return -1;
    }

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(8080);  // Set an appropriate port
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

    send_packet_pairs(sockfd, dest_addr, P / 8, spacing_ms, total_pairs);  // Convert P from bits to bytes

    close(sockfd);
    return 0;
}
