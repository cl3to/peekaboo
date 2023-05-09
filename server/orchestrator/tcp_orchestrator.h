#ifndef TCP_ORCHESTRATOR_H
#define TCP_ORCHESTRATOR_H

#include "orchestrator.h"

// Sent all data in more than one dataframe if necessary
// Returns 0 if all data was sent
// Returns -1 on failure
int send_all_data(int s, char *buf, int *len);

// Handle the requests from the client
// Return is void
void handle_client_requests(int new_fd);

// Handle connections using TCP socket
// Each orchestrator manages its own connections in its way
// Returns 0 if everything went well, 1 otherwise
int tcp_connection_loop(void);

#endif
