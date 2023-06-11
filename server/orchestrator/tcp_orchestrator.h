#ifndef TCP_ORCHESTRATOR_H
#define TCP_ORCHESTRATOR_H

#include "orchestrator.h"

// Handle connections using TCP socket
// Each orchestrator manages its own connections in its way
// Returns 0 if everything went well, 1 otherwise
int tcp_connection_loop(void);

#endif
