#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../../utils/constants.h"
#include "stub.h"


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
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int client_connect(ConnectionHandler *self)
{
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    // char *server_ip = getenv("SERVER_IP");

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
    return 0;
}

int client_send(ConnectionHandler *self, char *message)
{
    // Send data to server
    int message_length = strlen(message);
    int numbytes = send(self->sockfd, message, message_length, 0);

    if (numbytes == -1)
    {
        perror("send");
        return -1;
    }
    // fprintf(stderr, "CLIENT SIDE --> send bytes: %d\n", numbytes);
    return 0;
}

char* client_receive(ConnectionHandler *self)
{
    char *response = calloc(MAXDATASIZE, 1); // Allocating memory for response
    char buffer[MAXDATASIZE] = {0};
    int depth = 0, start = 0, end = 0, numbytes = 0;

    // Receive data from server
    do
    {
        numbytes = recv(self->sockfd, buffer, MAXDATASIZE, 0);
        if (numbytes < 0)
        {
            perror("recv");
            break;
        }
        // fprintf(stderr, "CLIENT SIDE --> receve bytes: %d\n", numbytes);

        start = end;
        end += numbytes;
        strncpy(response + start, buffer, numbytes);
        check_received_message(response, &depth, start, end);
    }
    while (depth);

    // printf("CLIENT SIDE --> received: %s\n", response);
    return response;
}

void client_disconnect(ConnectionHandler *self)
{
    close(self->sockfd);
}

char *make_request(ConnectionHandler *connection, char *request)
{
    char *response;
    connection->send(connection, request);
    response = connection->receive(connection);
    return response;
}
