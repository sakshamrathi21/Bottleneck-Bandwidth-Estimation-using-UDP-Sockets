#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

#define BUFFER_SIZE 1024

// Function to calculate time difference in microseconds using Windows API
double time_diff(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER frequency) {
    return ((double)(end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart);
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

    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int client_addr_len = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return -1;
    }

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed. Error Code: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Use the appropriate port
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the port
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return -1;
    }

    // Variables for high-resolution timing
    LARGE_INTEGER t1, t2, frequency;
    QueryPerformanceFrequency(&frequency);

    int packet_number = 0;
    while (1) {
        // Receive packet
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &client_addr_len);
        if (recv_len == SOCKET_ERROR) {
            printf("recvfrom failed. Error Code: %d\n", WSAGetLastError());
            break;
        }

        // For the first packet in the pair, store the receive time
        if (packet_number % 2 == 0) {
            QueryPerformanceCounter(&t1);  // Record time for the first packet
        } 
        // For the second packet in the pair, compute the time difference
        else {
            QueryPerformanceCounter(&t2);  // Record time for the second packet
            double delta_t = time_diff(t1, t2, frequency);  // Time difference in microseconds
            double C = (BUFFER_SIZE * 8) / delta_t;  // Assuming BUFFER_SIZE in bytes
            fprintf(fp, "%lf\n", C);  // Log result
        }

        packet_number++;
    }

    // Cleanup
    fclose(fp);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
