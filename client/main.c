#include <stdio.h>

#include "stub/stub.h"
#include "ui/peekaboo_tui.h"
#include "../utils/constants.h"

#define SERVER_IP_FLAG "--server"
#define TCP_FLAG "--tcp"
#define HELP_FLAG "--help"

char *help_message = "Usage: client [OPTIONS]\n"
                     "Options:\n"
                     "  " SERVER_IP_FLAG " <IP>        Connect to the server at <IP>\n"
                     "  " TCP_FLAG "                Use TCP sockets (UDP sockets are used by default)\n"
                     "  " HELP_FLAG "               Display this help message";

int main(int argc, char *argv[])
{
    char *server_ip = "127.0.0.1";
    char *server_port = PORT;
    int connected;
    char socket_type = 0;
    char message_buffer[512];

    // Parse the command line arguments
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], SERVER_IP_FLAG))
        {
            if (i + 1 < argc)
                server_ip = argv[++i];
            else
            {
                sprintf(message_buffer, "Error: %s flag requires an argument\n", SERVER_IP_FLAG);
                peekaboo_tui_message(message_buffer);
                return -1;
            }
        }
        else if (!strcmp(argv[i], TCP_FLAG))
        {
            socket_type = 1;
            socket_type++;
        }
        else if (!strcmp(argv[i], HELP_FLAG))
        {
            peekaboo_tui_message(help_message);
            return 0;
        }
        else
        {
            sprintf(message_buffer, "Error: Unrecognized flag %s", argv[i]);
            peekaboo_tui_message(message_buffer);
            return -1;
        }
    }

    if (argc == 2)
        server_ip = argv[1];

    ConnectionHandler conn_handler = {
        .server_ip = server_ip,
        .server_port = server_port,
        .connect = client_connect,
        .send = client_send,
        .receive = client_receive,
        .disconnect = client_disconnect,
    };

    // Connect to the server
    connected = conn_handler.connect(&conn_handler);

    // If the connection fails, exit
    if (connected == -1)
    {
        fail_connection();
        return -1;
    }

    // Enter the Peekaboo TUI
    peekaboo_tui(&conn_handler);

    // Disconnect from the server
    conn_handler.disconnect(&conn_handler);
    return 0;
}