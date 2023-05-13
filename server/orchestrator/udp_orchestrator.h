#ifndef UDP_ORCHESTRATOR_H
#define UDP_ORCHESTRATOR_H

#include "orchestrator.h"

// Handle connections using UDP socket
// Each orchestrator manages its own connections in its way
// Returns 0 if everything went well, 1 otherwise
int udp_connection_loop(void);

#endif
