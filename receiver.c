// This is the receiver code which receives the packets from the sender and calculates the bandwidth for each pair of packets.

// Importing necessary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

// Defining the buffer size
#define BUFFER_SIZE 1024

double time_diff(struct timeval start, struct timeval end) {
    // Calculates the time difference in microseconds
    return (double)(end.tv_sec - start.tv_sec) * 1000000 + (double)(end.tv_usec - start.tv_usec);
}

int main(int argc, char *argv[]) {
    // This is the main file. It expects the output file name as an argument.
    if (argc != 2) {
        // If the number of arguments is not 2, print the usage and return -1
        printf("Usage: %s <output_file>\n", argv[0]);
        return -1;
    }
    char *output_file = argv[1];
    FILE *fp = fopen(output_file, "w"); // Open the file in writing mode
    if (!fp) {
        // If the file opening fails, print an error message and return -1
        perror("File opening failed");
        return -1;
    }
    // Create a UDP socket
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  // UDP socket
    if (sockfd == -1) {
        // If the socket creation fails, print an error message and return -1
        perror("Socket creation failed");
        fclose(fp);
        return -1;
    }
    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Listening on port number 8080
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        // If the bind fails, print an error message, close the socket, close the file, and return -1
        perror("Bind failed");
        close(sockfd);
        fclose(fp);
        return -1;
    }
    printf("Listening on port 8080\n");

    struct timeval t1, t2; // Timings of the packets of a pair
    int previous_packet_number = -1; // Previous packet number
    ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len); // Receive the first packet which contains the packet size
    int packet_size = 0; // Size of the packets (P)
    for (int i = recv_len - 1 ; i >= 0 ; i --) {
        // Convert packet size from decimal format to integer
        if (buffer[i] == 'a') {
            continue;
        }
        packet_size = packet_size * 10 + buffer[i] - '0';
    }
    printf("Packet size: %d\n", packet_size);
    while (1) {
        ssize_t recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_len); // Receiving the buffer from the sender
        int packet_number = 0;
        if (buffer[0] == 'c') {
            // This means we have received the last packet, so we need to break out of this loop
            break;
        }
        // Converting the packet number from decimal format to integers
        for (int i = recv_len - 1 ; i >= 0 ; i --) {
            if (buffer[i] == 'a' || buffer[i] == 'b') {
                // Other subsidary characters in the buffer char array
                continue;
            }
            packet_number = packet_number * 10 + buffer[i] - '0';
        }
        printf("Received packet %d\n", packet_number);
        if (packet_number % 2 == 1) {
            // First packet of the pair
            previous_packet_number = packet_number;
            gettimeofday(&t1, NULL);  // Get time in microseconds
        } 
        else if (previous_packet_number == packet_number - 1) {
            gettimeofday(&t2, NULL); // Get time in microseconds
            double delta_t = time_diff(t1, t2);  // Time difference in microseconds
            double C = (packet_size*8) / delta_t;  // packet_size in bytes, so converting to bits
            fprintf(fp, "bandwidth: %lf %d %d\n", C, previous_packet_number, packet_number); // Printing the bandwidth of the current pair to the output file
        }
        else {
            // If the packet numbers are not consecutive, then there is a packet loss
            printf("Packet loss\n");
        }
    }

    fclose(fp); // Close the file
    close(sockfd); // Close the socket
    return 0;
}
