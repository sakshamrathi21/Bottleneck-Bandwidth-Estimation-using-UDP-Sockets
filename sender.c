// This is the code of the sender side of the UDP packet pairs experiment. The code is heavily commented and contains informative variable names for easier understanding.

// Importing necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define MAX_PACKET_BYTES 4

void send_packet_pairs(int sockfd, struct sockaddr_in dest_addr, int P, int spacing_ms, int total_pairs) {
    // This function sends a series of packet pairs to the receiver with the given spacing between them.
    // The first packet contains the packet size (P). This is useful for bandwidth calculation at the receiver side.
    char first_packet[P];
    for (int j = 0 ; j < P ; j ++) {
        first_packet[j] = 'a';
    }
    int packet_size = P, start_index = 0;
    // Converting the packet size to decimal format and storing it in the first packet
    while (packet_size > 0)
    {
        first_packet[start_index] = '0' + packet_size%10;
        packet_size = packet_size/10;
        start_index ++;
    }
    // Sending the first packet:
    sendto(sockfd, first_packet, sizeof(first_packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));

    // Iterating over all the packet pairs:
    for (int i = 1; i <= total_pairs; ++i) {
        // The first packet conatins all characters as 'a' except the first few characters which contain the packet number
        char packet1[P], packet2[P];
        for (int j = 0 ; j < P ; j ++) {
            packet1[j] = 'a';
        }
        int packet_num1 = 2*i-1, start_index = 0; // The packet id of the first packet in the pair
        // Converting the first packet number to decimal format
        while (packet_num1 > 0)
        {
            packet1[start_index] = '0' + packet_num1%10;
            packet_num1 = packet_num1/10;
            start_index ++;
        }
        // Sending the first packet
        sendto(sockfd, packet1, sizeof(packet1), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        printf("Packet %d\n", 2*i-1);

        // The second packet conatins all characters as 'b' except the first few characters which contain the packet number
        for (int j = 0 ; j < P ; j ++) {
            packet2[j] = 'b';
        }
        int packet_num2 = 2*i, start_index = 0;
        while (packet_num2 > 0) {
            packet2[start_index] = '0' + packet_num2%10;
            packet_num2 = packet_num2/10;
            start_index ++;
        }
        // Sending the second packet
        sendto(sockfd, packet2, sizeof(packet2), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        printf("Packet %d\n", 2*i);

        // Sleep for the given spacing time
        usleep(spacing_ms * 1000);
    }
    // Sending the last packet which contains all characters as 'c'. This helps the receiver to terminate automatically.
    char last_packet[P];
    for (int j = 0 ; j < P ; j ++) {
        last_packet[j] = 'c';
    }
    // Sending the last packet
    sendto(sockfd, last_packet, sizeof(last_packet), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
}

int main(int argc, char *argv[]) {
    // The main file expects 4 arguments: packet size in bits, destination IP, spacing between packet pairs in ms, and total number of packet pairs.
    if (argc != 5) {
        // Incorrect format, then will print the correct format and return -1
        printf("Usage: %s <packet_size_in_bits> <dest_ip> <spacing_ms> <total_packet_pairs>\n", argv[0]);
        return -1;
    }
    int P = atoi(argv[1]); // Packet size in bits
    char *dest_ip = argv[2]; // Destination IP
    int spacing_ms = atoi(argv[3]); // Spacing between packet pairs in ms
    int total_pairs = atoi(argv[4]); // Total number of packet pairs

    int sockfd;
    struct sockaddr_in dest_addr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // UDP socket
    if (sockfd == -1) {
        // Socket creation failed
        perror("Socket creation failed");
        return -1;
    }
    // Set up the destination address structure
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(8080);  // Set an appropriate port
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

    // We need to convert P bits to bytes (because we are using char array). 
    send_packet_pairs(sockfd, dest_addr, P / 8, spacing_ms, total_pairs); // Sending the packet pairs
    close(sockfd); // Closing the socket
    return 0;
}
