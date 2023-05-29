
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
int request_len = 0;

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

char* client_receive(ConnectionHandler *self)
{
    char *rbuffer = calloc(MAXDATASIZE, 1); // Allocating memory for receive buffer
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

    strcpy(rbuffer, packet_manager.buffer);
    packet_manager.destroy(&packet_manager);
    return rbuffer;
}

void client_disconnect(ConnectionHandler *self)
{
    close(self->sockfd);
}

char *make_request(ConnectionHandler *connection, Request *request)
{
    current_operation = request->operation_code;
    
    // NOTE: request_len is the length of the request string + 5 bytes for the
    // operation code and the JSON END
    // TODO: How deal with image data in the request?
    request_len = request->data_size;
    
    char *response = NULL;
    // printf("request:\n%s\n", request+5);
    connection->send(connection, request);
    response = connection->receive(connection);
    return response;
}

void init_packet_manager(PacketManager *self)
{
    self->buffer = calloc(MAXDATASIZE, sizeof(char));
    self->buffer_size = MAXDATASIZE;
    self->buffer_end = 0;
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

    if (op != DOWNLOAD_PROFILE_IMAGE)
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
        self->completed = 1;

        return 0;
    }

    else
    {   
        // TODO: Implement image packet inspection
        printf("Image packet inspection\n");
        
        // uint32_t p_data_size;

        // p_data_size = (
        //     (uint32_t) packet[1] << 24 |
        //     (uint32_t) packet[2] << 16 |
        //     (uint32_t) packet[3] << 8 |
        //     (uint32_t) packet[4]
        // );

        // self->npr++;
        // if (self->npr == self->nptr)
        // {
        //     self->complete = 1;
        // }
    }

    return 0;
}