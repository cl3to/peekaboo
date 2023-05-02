#ifndef EMAIL_VALIDATOR_H
#define EMAIL_VALIDATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <unistd.h>

// Checks whether the given email is valid or not
// Returns 0 indicates a valid email
// Returns non-zero values indicate an invalid email
int check_email_format(char *email);

#endif