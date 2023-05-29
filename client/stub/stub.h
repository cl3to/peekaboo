#ifndef STUB_H
#define STUB_H

#include <poll.h>

#include "../../utils/constants.h"

// Function prototypes
typedef struct connection_handler ConnectionHandler;
typedef int (*connect_function_t)(ConnectionHandler *self, int socktype);
typedef int (*send_function_t)(ConnectionHandler *self, Request *request);
typedef char* (*receive_function_t)(ConnectionHandler *self);
typedef void (*disconnect_function_t)(ConnectionHandler *self);

// Abstract the connection operations in the client side
struct connection_handler
{
    int socktype; // socket type
    int sockfd; // socket file descriptor
    char *server_ip;
    char *server_port;
    struct pollfd pollfd[1]; // monitor the socket with poll()
    connect_function_t connect; // Connect with the server
    send_function_t send;
    receive_function_t receive;
    disconnect_function_t disconnect;
};

// Start a connection with the server
// Return 0 if success, -1 otherwise
int client_connect(ConnectionHandler *self, int socktype);

// Send a message to the server
// Return 0 if success, -1 otherwise
int client_send(ConnectionHandler *self, Request *request);

// Receive a message from the server
// Return the message if success, NULL otherwise
char* client_receive(ConnectionHandler *self);

// disconnect the connection with the server
void client_disconnect(ConnectionHandler *self);

// Make a request to the server and return the response
char *make_request(ConnectionHandler *connection, Request *request);

// Manage receipt of packets by socket
typedef struct packet_manager PacketManager;

// Function prototypes for packet manager
typedef void (*init_packet_manager_function_t)(PacketManager *self);
typedef void (*destroy_packet_manager_function_t)(PacketManager *self);
typedef int (*inspect_packet_function_t)(PacketManager *self, char *packet, int packet_size, OperationCode op);

// Initialize the packet manager
void init_packet_manager(PacketManager *self);

// Destroy the packet manager
void destroy_packet_manager(PacketManager *self);

// Inspect the packet
// return 1 if the are an error in the packet, 0 otherwise
int inspect(PacketManager *self, char *packet, int packet_size, OperationCode op);

struct packet_manager
{
    char *buffer; // buffer to store the packets
    int buffer_size;
    int buffer_end; // index of the last byte in the buffer
    int nptr; // number of packets to receive
    int npr; // number of packets received
    char completed; // flag to indicate if data is completed received
    init_packet_manager_function_t init;
    destroy_packet_manager_function_t destroy;
    inspect_packet_function_t inspect;
};


#endif