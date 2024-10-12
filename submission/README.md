
# Bottleneck Bandwidth Estimation using UDP Sockets

This project implements a network measurement tool that estimates the bottleneck bandwidth of a network using UDP sockets. It consists of two programs: `sender.c` and `receiver.c`.

## Compilation Instructions

### Receiver Program
1. Compile the receiver program using the following command:
   ```bash
   gcc receiver.c -o receiver
   ```

2. Run the receiver program:
   ```bash
   ./receiver <buffer-file-name>
   ```
   - `<buffer-file-name>`: The file where the received packets' information will be stored.

### Sender Program
1. Compile the sender program using the following command:
   ```bash
   gcc sender.c -o sender
   ```

2. Run the sender program:
   ```bash
   ./sender <packet-size-in-bits> <destination-ip-address> <spacing-time-in-ms> <total-number-of-packet-pairs>
   ```
   - `<packet-size-in-bits>`: Size of each packet being sent.
   - `<destination-ip-address>`: The IPv4 address of the machine running the receiver program.
   - `<spacing-time-in-ms>`: Time interval between sending each packet pair, in milliseconds.
   - `<total-number-of-packet-pairs>`: The total number of packet pairs to be sent.

## Example Usage

To run the receiver:
```bash
./receiver data_buffer.txt
```

To run the sender:
```bash
./sender 1024 192.168.1.2 10 100
```

This will send 100 packet pairs of size 1024 bits each to the receiver at IP address `192.168.1.2`, with a spacing of 10 milliseconds between packet pairs.

## Overview

The sender sends UDP packets to the receiver based on the parameters specified by the user, while the receiver measures the time intervals between the arrival of the packets. Using this data, the bottleneck bandwidth can be estimated based on the spacing between packets at the receiver side.
