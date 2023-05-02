#ifndef STUB_H
#define STUB_H

// Function prototypes
typedef struct connection_handler ConnectionHandler;
typedef int (*connect_function_t)(ConnectionHandler *self) ;
typedef int (*send_function_t)(ConnectionHandler *self, char *message);
typedef char* (*receive_function_t)(ConnectionHandler *self);
typedef void (*disconnect_function_t)(ConnectionHandler *self);

// Abstract the connection operations in the client side
struct connection_handler
{
    int sockfd; // socket file descriptor
    char *server_ip;
    char *server_port;
    connect_function_t connect; // Connect with the server
    send_function_t send;
    receive_function_t receive;
    disconnect_function_t disconnect;
};

// Start a connection with the server
// Return 0 if success, -1 otherwise
int client_connect(ConnectionHandler *self);

// Send a message to the server
// Return 0 if success, -1 otherwise
int client_send(ConnectionHandler *self, char *message);

// Receive a message from the server
// Return the message if success, NULL otherwise
char* client_receive(ConnectionHandler *self);

// disconnect the connection with the server
void client_disconnect(ConnectionHandler *self);

// Make a request to the server and return the response
char *make_request(ConnectionHandler *connection, char *request);

#endif