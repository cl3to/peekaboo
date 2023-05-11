#include "udp_orchestrator.h"

// Handle the connection with the client
// Return is void
void _handle_udp_client_requests(int sockfd)
{
    // Create profiles variables
    // The BUFFER_SIZE limit the maximum number of profiles in the system
    const int BUFFER_SIZE = sizeof(Profile) * 64;
    Profile *profiles = (Profile *)malloc(BUFFER_SIZE);

    // Create connection variables
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    // Create messages variables
    // The MAXDATASIZE limit the maximum number of bytes we can get at once
    char request[MAXDATASIZE + 1];
    char *response;
    int request_len, response_len, sent_bytes;
    int send_failures_count = 0;

    // Process ID for prints log of the server
    int pid = getpid();

    // Loop to process the requests
    while (1)
    {
        // Receive request
        addr_len = sizeof their_addr;
        if ((request_len = recvfrom(sockfd, request, MAXDATASIZE - 1, 0,
                                    (struct sockaddr *)&their_addr, &addr_len)) == -1)
        {
            printf("(pid %d) SERVER >>> ", pid);
            perror("recvfrom");

            exit(1);
        }

        // TODO: Maybe remove this
        printf("(pid %d) SERVER >>> Got packet from %s\n",
               pid,
               inet_ntop(their_addr.ss_family,
                         get_in_addr((struct sockaddr *)&their_addr),
                         s, sizeof s));

        request[request_len] = '\0';

        printf("(pid %d) SERVER >>> Receive packet with %d bytes\n", pid, request_len);
        printf("(pid %d) SERVER >>> Receive packet contains: '%s'\n", pid, request);

        // Handle request from client using serializers
        response = general_serializer(profiles, request);
        response_len = strlen(response);

        printf("(pid %d) SERVER >>> Sending response with %d bytes\n", pid, response_len);
        printf("(pid %d) SERVER >>> Sending response message: '%s'\n", pid, response);

        // Send response message
        if ((sent_bytes = sendto(sockfd, response, response_len, 0,
                                 (struct sockaddr *)&their_addr, addr_len)) == -1)
        {
            printf("(pid %d) SERVER >>> ", pid);
            perror("sendto");

            send_failures_count++;
            printf("(pid %d) SERVER >>> Total number of unsuccessful message sends to clients: %d\n", pid, send_failures_count);

            exit(1);
        }

        printf("(pid %d) SERVER >>> Sent %d bytes to client \n", pid, sent_bytes);

        // Check if the server sent the complete message
        if (response_len != sent_bytes)
        {
            send_failures_count++;
            printf("(pid %d) SERVER >>> FAILED TO SEND MESSAGE. %d bytes sent out of %d bytes\n", pid, sent_bytes, response_len);
            printf("(pid %d) SERVER >>> Total number of unsuccessful message sends to clients: %d\n", pid, send_failures_count);
        }
    }

    free(response);
    free(profiles);
}

int udp_connection_loop(void)
{
    // Define socket variables
    int sockfd;
    struct addrinfo hints, *servinfo;
    int rv;
    // Process ID for prints log of the server
    int pid = getpid();

    // Define socket parameters
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to use IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    sockfd = setup_socket(servinfo);
    freeaddrinfo(servinfo); // all done with this structure

    printf("(pid %d) SERVER >>> waiting for connections...\n", pid);

    // Create the shared memory segment to store password hash
    /*
    In this case a shared memory segment is not necessary,
    but for convenience, the same TCP create_shared_memory_buffer
    function was used to structure the token
    */
    if (create_shared_memory_buffer() == -1)
    {
        return 1;
    }

    // handle with connections
    _handle_udp_client_requests(sockfd);

    close(sockfd);
    return 0;
}
