#include "orchestrator/tcp_orchestrator.h"
#include "orchestrator/udp_orchestrator.h"

// Global variables to manager the type of connection
int should_use_tcp = 0; // 0 is UDP connection and 1 is TCP connection

// The help message
char *help_message = "Usage: server [OPTIONS]\n"
                     "Options:\n"
                     "  " TCP_FLAG "                Use TCP sockets (UDP sockets are used by default)\n"
                     "  " HELP_FLAG "               Display this help message";

// Start the server calling connection_loop function
int main(int argc, char *argv[])
{

    // loop through command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], TCP_FLAG))
        {
            should_use_tcp = 1;
        }
        else if (!strcmp(argv[i], HELP_FLAG))
        {
            printf("%s\n", help_message);
            return 0;
        }
        else
        {
            printf("Error: Unrecognized flag %s\n", argv[i]);
            return -1;
        }
    }

    // Start the chosen server
    if (should_use_tcp)
    {
        printf("SERVER >>> Starting TCP server...\n");
        return tcp_connection_loop();
    }
    printf("SERVER >>> Starting UDP server...\n");
    return udp_connection_loop();
}