#ifndef STUB_H
#define STUB_H

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

#define MAXDATASIZE 9360 // max number of bytes we can get at once 

// Make a request to the server and return the response

int make_request(char *request);

#endif