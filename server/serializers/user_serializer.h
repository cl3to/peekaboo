#ifndef USER_SERIALIZER_H
#define USER_SERIALIZER_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#include "../../utils/email_validator.h"
#include "../../utils/user_message.h"
#include "../database/database.h"

// Global variables to manager the type of connection
extern int should_use_tcp;

// Serialization of the profiles filtred by the academic degree
// Returns the profiles in json format
// Returns -1 if error occurred
response_stream *profiles_by_course_serializer(Profile *profiles, char *course);

// Serialization of the profiles filtred by the skills
// Returns the profiles in json format
// Returns -1 if error occurred
response_stream *profiles_by_skill_serializer(Profile *profiles, char *skills);

// Serialization of the profiles filtred by the year of degree
// Returns the profiles in json format
// Returns -1 if error occurred
response_stream *profiles_by_year_of_degree_serializer(Profile *profiles, int year_of_degree);

// Serialization of all profiles in the database
// Returns the profiles in json format
// Returns -1 if error occurred
response_stream *profiles_serializer(Profile *profiles);

// Serialization of one profile filteres by e-mail
// Returns the profiles in json format
// Returns -1 if error occurred
response_stream *profile_by_email_serializer(Profile *profile, char *email);

// Serialization of a profile image
// Returns the image and a header of 8 bytes
// Returns a error response if error occurred
response_stream *image_by_email(char *email);

#endif