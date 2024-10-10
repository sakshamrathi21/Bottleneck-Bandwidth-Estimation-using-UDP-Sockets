#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

void send_packet_pairs(int sockfd, struct sockaddr_in dest_addr, int P, int spacing_ms, int total_pairs) {
    
    for (int i = 1; i <= total_pairs; ++i) {
        // Packet 1
        // printf("Packet %d\n", i);
        char packet1[P];
        for (int j = 0 ; j < P ; j ++) {
            packet1[j] = 'a';
        }
        int packet_num1 = 2*i-1;
        // for (int j = 0; j < sizeof(int); ++j) {
        //     packet1[j] = (packet_num1 >> (8 * j)) & 0xFF;  // Extract 8 bits at a time
        // }
        sendto(sockfd, packet1, sizeof(packet1), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        printf("Packet %d\n", 2*i-1);
        for (int j = 0 ; j < P ; j ++) {
            packet1[j] = 'b';
        }
        int packet_num2 = 2*i;
        // for (int j = 0; j < sizeof(int); ++j) {
        //     packet1[j] = (packet_num2 >> (8 * j)) & 0xFF;  // Extract 8 bits at a time
        // }
        sendto(sockfd, packet1, sizeof(packet1), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        printf("Packet %d\n", 2*i);
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
    // printf("total_pairs: %d\n", total_pairs);
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
    printf("hello\n");
    send_packet_pairs(sockfd, dest_addr, P / 8, spacing_ms, total_pairs);  // Convert P from bits to bytes

    close(sockfd);
    return 0;
}
