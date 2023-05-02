#include "orchestrator.h"

// Collects any terminated child processes to prevent them from becoming zombies
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    // Collect any terminated child processes using waitpid() with WNOHANG option
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// Setup socket for connection
int setup_socket(struct addrinfo *servinfo)
{
    int sockfd;
    struct addrinfo *p;
    int yes = 1;

    // Loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        // Create a new socket
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        // Allow the socket to reuse the address
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            return 1;
        }

        // Bind the socket to the specified address and port number
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            // if the bind fails, close the socket and print an error message
            close(sockfd);
            perror("server: bind");
            continue;
        }

        // If we successfully bind to an address, break out of the loop
        break;
    }

    // Retunr 1 if the bind failed
    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        return 1;
    }

    // Return the socket file descriptor
    return sockfd;
}

// Create a shared memory segment
int create_shared_memory_buffer()
{
    int shmid = shmget(IPC_PRIVATE, SESSION_TOKEN_LENGTH + 1, IPC_CREAT | 0666);

    if (shmid < 0)
    {
        perror("Error creating shared memory segment");
        return -1;
    }

    // Assign the shared memory segment to expected_session_token global variable
    expected_session_token = (char *)shmat(shmid, NULL, 0);

    if (expected_session_token == (char *)-1)
    {
        perror("Error assigning shared memory segment to variable");
        return -1;
    }
    return 0;
}

// Sent all data in more than one dataframe if necessary
int send_all_data(int s, char *buf, int *len)
{
    int total = 0;        // how many bytes we've sent
    int bytesleft = *len; // how many we have left to send
    int n;

    while (total < *len)
    {
        n = send(s, buf + total, bytesleft, 0);
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }

    *len = total; // return number actually sent here

    return n == -1 ? -1 : 0; // return -1 on failure, 0 on success
}

// Handle the connection with the client
void handle_client_requests(int new_fd)
{
    // Create profiles variables
    // The BUFFER_SIZE limit the maximum number of profiles in the system
    const int BUFFER_SIZE = sizeof(Profile) * 64;
    Profile *profiles = (Profile *)malloc(BUFFER_SIZE);

    // Create messages variables
    // The MAXDATASIZE limit the maximum number of bytes we can get at once
    char request[MAXDATASIZE + 1];
    char *response;
    int request_len, response_len;

    // Process ID for prints log of the server
    int pid = getpid();

    // Handle requests from client without closing the connection
    while (1)
    {
        if ((request_len = recv(new_fd, request, MAXDATASIZE, 0)) == -1)
        {
            printf("(pid %d) SERVER >>> ", pid);
            perror("recv");
            exit(1);

            free(response);
            free(profiles);

            break;
        }

        // check the end of the connection
        if (request_len == 0)
        {
            printf("(pid %d) SERVER >>> Connection closed.\n", pid);

            free(response);
            free(profiles);

            break;
        }

        request[request_len] = '\0';

        printf("(pid %d) SERVER >>> Receive request with %d bytes\n", pid, request_len);
        printf("(pid %d) SERVER >>> Receive request message: '%s'\n", pid, request);

        // Handle request from client using serializers
        response = general_serializer(profiles, request);
        response_len = strlen(response);

        printf("(pid %d) SERVER >>> Sending response with %d bytes\n", pid, response_len);
        printf("(pid %d) SERVER >>> Sending response message: '%s'\n", pid, response);

        // Send response message
        if (send_all_data(new_fd, response, &response_len) == -1)
        {
            printf("(pid %d) SERVER >>> ", pid);
            perror("sendall");
            printf("(pid %d) SERVER >>> Only sent %d bytes because of the error!\n", pid, response_len);
        }
    }
}

int connection_loop(void)
{
    // Define socket variables
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    char s[INET6_ADDRSTRLEN];
    int rv;

    // Define socket parameters
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    sockfd = setup_socket(servinfo);
    freeaddrinfo(servinfo); // all done with this structure

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        return 1;
    }

    // Handle with dead processes
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        return 1;
    }

    printf("server: waiting for connections...\n");

    // Create the shared memory segment to store password hash
    if (create_shared_memory_buffer() == -1)
    {
        return 1;
    }

    while (1)
    {
        // Main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        // Show network address on terminal
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork())
        {
            // this is the child process
            close(sockfd); // child doesn't need the listener

            // handle with this connection in this process
            handle_client_requests(new_fd);

            // close the socker with the client
            close(new_fd);
            exit(0);
        }
        close(new_fd); // parent doesn't need this
    }

    return 0;
}