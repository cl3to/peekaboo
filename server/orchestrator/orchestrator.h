
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

// #include "model/profile.h"

#define PORT "4527" // the port client will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

// Handle connections using TCP or UDP sockets
// Each orchestrator manages its own connections in its way
// Returns 0 if everything went well, 1 otherwise
int connection_loop(void);