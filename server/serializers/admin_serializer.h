#ifndef ADMIN_SERIALIZER_H
#define ADMIN_SERIALIZER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>

#include "../database/database.h"
#include "../../utils/constants.h"
#include "../../utils/email_validator.h"

// Generate a token to the current serssion
// Returns 1 after instantiation the new token
int generate_session_token();

// Validate the given session token
// Returns 0 if the token is valid
// Returns -1 if the token is invalid
int validate_session_token(char *session_token);

// Validate the given password (using a hardcode hash)
// Returns a session token if the password is valid
// Returns NULL if the password is invalid
char* validate_password(char *password);

// Login to the system's administrative area
// Returns a session token if the login is valid
// Returns NULL if the login is invalid
char* login_with_password(char *password);

// Logout from the system's administrative area
// Returns 0 if the session was finished
// Returns -1 if there was an error in the session finalization
int logout(char *session_token);

// Create a new profile 
// Returns 0 if the profile was created
// Returns -1 if there was an error
int create_new_profile(char *session_token, char *email, char *name, char *last_name, char *city, char *academic_degree, int year_of_degree, char *habilities);

// Remove a profile filter from email
// Returns 0 if the profile was removed
// Returns -1 if there was an error
int remove_profile_by_email(char *session_token, char *email);

#endif 