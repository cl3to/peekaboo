#ifndef PEAKABOO_TUI_H
#define PEAKABOO_TUI_H

#include "../../model/profile.h"
#include "../../utils/constants.h"

// Peekaboo TUI
void peekaboo_tui(int argc, char **argv);

// Peekaboo TUI - User Mode
void user_mode_tui();

// Peekaboo TUI - Admin Mode
void admin_mode_tui();

// Peekaboo TUI - Print Profile
void print_profile(Profile *profile, int data_len, OperationCode operation_code);

#endif // PEAKABOO_TUI_H