#include "stub/stub.h"
#include "ui/peekaboo_tui.h"
#include "../utils/constants.h"

int main(int argc, char *argv[])
{
    char *server_ip = getenv("SERVER_IP");
    char *server_port = PORT;
    int connected;

    if (argc == 3)
    {
        server_ip = argv[1];
        server_port = argv[2];
    }

    ConnectionHandler conn_handler = {
        .server_ip = server_ip,
        .server_port = server_port,
        .connect = client_connect,
        .send = client_send,
        .receive = client_receive,
        .disconnect = client_disconnect,
    };

    connected = conn_handler.connect(&conn_handler);

    if (connected == -1)
    {
        fail_connection();
        return -1;
    }

    peekaboo_tui(&conn_handler);
    return 0;
}