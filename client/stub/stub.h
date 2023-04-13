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

#define PORT "4527" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// Handle client connections using TCP or UDP sockets
// Each stub manages its own connections in its way
// Returns 0 if everything went well, 1 otherwise
int client_loop(int argc, char *argv[]);