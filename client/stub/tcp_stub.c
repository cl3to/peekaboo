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
    for(int i = start; i < end; i++)
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

char* make_request(char *request)
{
    int sockfd, numbytes;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char *server_ip = getenv("SERVER_IP");

    if ((rv = getaddrinfo(server_ip, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return NULL;
    }

    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return NULL;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);

    // fprintf(stderr, "client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    // Send data to server
    int data_len = strlen(request);
    if ((numbytes = send(sockfd, request, data_len, 0)) == -1)
        perror("send");

    // fprintf(stderr, "CLIENT SIDE --> send bytes: %d\n", numbytes);
    
    char* response = calloc(MAXDATASIZE, 1); // Allocating memory for response
    char buffer[BUFFER_SIZE] = {0};
    int depth = 0, start = 0, end = 0, received = 0, recall = 10;

    // Receive data from server
    while(!received)
    {
        numbytes = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (numbytes < 0)
        {
            perror("recv");
            exit(1);
        }

        // fprintf(stderr, "CLIENT SIDE --> receve bytes: %d\n", numbytes);

        start = end;
        end += numbytes;
        strncpy(response + start, buffer, numbytes);
        check_received_message(response, &depth, start, end);

        if (depth == 0)
            received = 1;

        recall = (numbytes == 0) ? recall - 1 : recall;
        if (recall == 0)
            break;
    }

    close(sockfd);

    return response;
}