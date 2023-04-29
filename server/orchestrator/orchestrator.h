#ifndef ORCHESTRATOR_H
#define ORCHESTRATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/shm.h>

#include "../serializers/general_serializer.h"
#include "../../model/profile.h"
#include "../../utils/constants.h"

// how many pending connections queue will hold on the server
#define BACKLOG 10 


// Collects any terminated child processes to prevent them from becoming zombies
// Return is void
void sigchld_handler(int s);

// Get sockaddr, IPv4 or IPv6:
// Returns pointer to the sin_addr or sin6_addr member of a sockaddr_in or sockaddr_in6 structure
void *get_in_addr(struct sockaddr *sa);

// Setup socket for connection
// Returns socket file descriptor on success
// Returns -1 on failure
int setup_socket(struct addrinfo *servinfo);

// Create a shared memory segment
// Returns 0 if the segment is already
// Returns -1 on failure
int create_shared_memory_buffer();

// Sent all data in more than one dataframe if necessary
// Returns 0 if all data was sent
// Returns -1 on failure
int send_all_data(int s, char *buf, int *len);

// Handle the requests from the client
// Return is void
void handle_client_requests(int new_fd);

// Handle connections using TCP or UDP sockets
// Each orchestrator manages its own connections in its way
// Returns 0 if everything went well, 1 otherwise
int connection_loop(void);

#endif