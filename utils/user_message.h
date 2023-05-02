#ifndef USER_MESSAGE_H
#define USER_MESSAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "../model/profile.h"
#include "constants.h"

// Build a user response message in json format
// Return a user message in json format
// In case of error, return a empty message in the waiting format
char *make_user_response_msg(struct profile *profiles, int profiles_amount, OperationCode operation_code);

#endif