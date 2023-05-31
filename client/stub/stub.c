
// system libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

// Network libraries
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "stub.h"

#define TIMEOUT 5000 // 5 seconds

// TODO: Improve context variables
OperationCode current_operation = EXIT;

// Check if the received message is valid
// Parameters:
//  - message: the message to be checked
//  - depth: the current depth of the message
//  - start: the index of the first character of the message
//  - end: the index of the last character of the message
void check_received_message(char *message, int *depth, int start, int end)
{
    char c;
    for (int i = start; i < end; i++)
    {
        c = message[i];
        if (c == '{' || c == '[')
            (*depth)++;
        else if (c == '}' || c == ']')
            (*depth)--;
    }
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int client_connect(ConnectionHandler *self, int socktype)
{
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;

    // If socktype is 1, use TCP sockets, otherwise use UDP sockets
    self->socktype = (socktype) ? SOCK_STREAM : SOCK_DGRAM;
    hints.ai_socktype = self->socktype;
    
    if ((rv = getaddrinfo(self->server_ip, self->server_port, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((self->sockfd = socket(p->ai_family, p->ai_socktype,
                                   p->ai_protocol)) == -1)
        {
            perror("client");
            continue;
        }

        if (connect(self->sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(self->sockfd);
            perror("client");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return -1;
    }

    freeaddrinfo(servinfo); // all done with this structure

    self->pollfd[0].fd = self->sockfd;
    self->pollfd[0].events = POLLIN; // check ready-to-read

    return 0;
}

int client_send(ConnectionHandler *self, Request *request)
{
    // Send data to server
    int numbytes = send(self->sockfd, request->data, request->data_size, 0);

    if (numbytes == -1)
    {
        perror("send");
        return -1;
    }

    return 0;
}

Response* client_receive(ConnectionHandler *self)
{
    char *rbuffer = calloc(MAX_IMAGE_SIZE, 1); // Allocating memory for receive buffer
    Response *response = malloc(sizeof(Response));

    int received_bytes = 0, inspect_status = 0;
    PacketManager packet_manager = {
        .init = init_packet_manager,
        .destroy = destroy_packet_manager,
        .inspect = inspect,
    };

    packet_manager.init(&packet_manager);

    // TODO: Wait for image data packets from server
    while(!packet_manager.completed)
    {
        int rv = poll(self->pollfd, 1, TIMEOUT);
        if (rv == -1)
        {
            perror("poll"); // error occurred in poll()
            break;
        }
        else if (rv == 0)
        {
            fprintf(stderr, "Timeout occurred! No data after %d seconds.\n", TIMEOUT / 1000);
            break;
        }
        else
        {
            if (self->pollfd[0].revents & POLLIN)
            {

                received_bytes = recv(self->sockfd, rbuffer, MAXDATASIZE, 0);
                if (received_bytes < 0)
                {
                    perror("recv");
                    break;
                }

                // printf("received buffer:\n%s\n", rbuffer);
                // printf("inspected buffer:\n%s\n", packet_manager.buffer);
                inspect_status = packet_manager.inspect(
                    &packet_manager, rbuffer, received_bytes, current_operation
                );
                // printf("inspected buffer:\n%s\n", packet_manager.buffer);

                if (inspect_status)
                {
                    fprintf(stderr, "failed to inspect packet.\n");
                    break;
                }
            }
            else if (self->pollfd[0].revents & POLLERR)
            {
                fprintf(stderr,"Error establishing communication with the server.\n");
                break;
            }
        }
    }

    memcpy(rbuffer, packet_manager.buffer, packet_manager.used_size);
    response->data = rbuffer;
    response->data_size = packet_manager.used_size;
    packet_manager.destroy(&packet_manager);
    return response;
}

void client_disconnect(ConnectionHandler *self)
{
    close(self->sockfd);
}

Response *make_request(ConnectionHandler *connection, Request *request)
{
    // Save current operation code in the stub context   
    current_operation = request->operation_code;
    Response *response = NULL;

    connection->send(connection, request);
    response = connection->receive(connection);
    return response;
}

void init_packet_manager(PacketManager *self)
{
    self->buffer = calloc(MAX_IMAGE_SIZE, sizeof(char));
    self->buffer_size = MAX_IMAGE_SIZE;
    self->used_size = 0;
    self->buffer_end = 0;
    // self->received_packets = calloc(MAX_SENT_PACKETS>>3, sizeof(uint8_t));
    self->nptr = MAX_SENT_PACKETS;
    self->npr = 0;
    self->completed = 0;
}

void destroy_packet_manager(PacketManager *self)
{
    free(self->buffer);
}

int inspect(PacketManager *self, char *packet, int packet_size, OperationCode op)
{

    if (packet_size <= 0)
    {
        fprintf(stderr, "The packet size (%d) is invalid.\n", packet_size);
        return 1;
    }

    if (op == DOWNLOAD_PROFILE_IMAGE)
    {   
        // TODO: Implement image packet inspection
        // printf("Image packet inspection\n");
        
        // Packet Number (0-total_packets)
        uint8_t current_packet = (uint8_t) packet[0];
        // Number of packets to receive
        uint8_t total_packets = (uint8_t) packet[1];
        // Size of the data Field in the packet
        uint16_t data_size = ((uint16_t) packet[2] << 8) | ((uint16_t) packet[3]);
        // Image size
        uint32_t image_size = (
            (uint32_t) packet[4] << 24 |
            (uint32_t) packet[5] << 16 |
            (uint32_t) packet[6] << 8 |
            (uint32_t) packet[7]
        );

        // Set the number of packets to receive
        if (current_packet == 0)
            self->nptr = total_packets;

        // Posição do inicial do pacote no buffer (na imagem)
        int p_index = (image_size / total_packets) * current_packet;

        // Copy the packet data to the correct position in the image buffer
        memcpy(self->buffer + p_index, packet + 8, data_size);
        self->npr++;
        
        // Update the used size
        self->used_size += data_size;

        // TODO: Use a bitmap to manage incoming packets if needed
        // self->received_packets[current_packet >> 3] |= (((uint8_t) 1) << (current_packet & 7));

        if (self->npr == self->nptr)
        {
            self->used_size = image_size;
            self->completed = 1;
        }
    }

    else
    {
        // Copy data to buffer
        memcpy(self->buffer + self->buffer_end, packet, packet_size);
        self->buffer_end += packet_size;

        int depth = 0;
        check_received_message(self->buffer, &depth, 0, self->buffer_end);

        if (depth)
        {
            fprintf(stderr, "Incomplete JSON message.\n");
            return 1;
        }
    
        // Set packet receive as complete
        self->used_size = packet_size;
        self->completed = 1;

        return 0;
    }
    return 0;
}