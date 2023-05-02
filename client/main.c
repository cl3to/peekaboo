#include "stub/stub.h"
#include "ui/peekaboo_tui.h"
#include "../utils/constants.h"

int main(int argc, char *argv[])
{
    char *server_ip = "127.0.0.1";
    char *server_port = PORT;
    int connected;

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