#ifndef ADMIN_SERIALIZER_H
#define ADMIN_SERIALIZER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "../../lib/sha1-c/sha1.h"

#include "../database/database.h"
#include "../../utils/constants.h"
#include "../../utils/email_validator.h"

// Global variable to handle the session token
// This variable is shared between the processes
extern char *expected_session_token;

// Generate a token to the current serssion
// Returns 1 after instantiation the new token
int generate_session_token();

// Validate the given session token
// Returns 0 if the token is valid
// Returns -1 if the token is invalid
int validate_session_token(char *session_token);

// Validate the given password
// Returns a session token if the password is valid
// Returns NULL if the password is invalid
char *validate_password(char *password);

// Login to the system's administrative area
// Returns a admin response in JSON format with a Status Code of success (200) or error (>=400)
response_stream *login_with_password(char *password);

// Logout from the system's administrative area
// Returns a admin response in JSON format with a Status Code of success (200) or error (>=400)
response_stream *logout(char *session_token);

// Create a new profile
// Returns a admin response in JSON format with a Status Code of success (200) or error (>=400)
response_stream *create_new_profile(char *session_token, char *email, char *name, char *last_name, char *city, char *course, int year_of_degree, char *skills, char *image, int image_size);

// Remove a profile filter from email
// Returns a admin response in JSON format with a Status Code of success (200) or error (>=400)
response_stream *remove_profile_by_email(char *session_token, char *email);

#endif