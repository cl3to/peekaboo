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

#include "../../utils/constants.h"

#define BUFFER_SIZE 1500

// Make a request to the server and return the response

char* make_request(char *request);

#endif