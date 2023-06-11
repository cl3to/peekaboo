#ifndef PEAKABOO_TUI_H
#define PEAKABOO_TUI_H

#include "../../model/profile.h"
#include "../stub/stub.h"

// Peekaboo TUI
void peekaboo_tui(ConnectionHandler *conn_handler);

// Peekaboo TUI - FAIL CONNECTION MESSAGE
void fail_connection(void);

// Peekaboo TUI - MESSAGE
void peekaboo_tui_message(char *message);

#endif // PEAKABOO_TUI_H