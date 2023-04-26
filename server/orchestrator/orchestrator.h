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

#define BACKLOG 10 // how many pending connections queue will hold

// Handle connections using TCP or UDP sockets
// Each orchestrator manages its own connections in its way
// Returns 0 if everything went well, 1 otherwise
int connection_loop(void);

#endif