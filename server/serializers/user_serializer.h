#ifndef USER_SERIALIZER_H
#define USER_SERIALIZER_H

#include <stdlib.h>
#include <string.h>

#include "../../utils/email_validator.h"

// Serialization of the profiles filtred by the academic degree 
// Returns the profiles in json format
// Returns -1 if error occurred
char* profiles_by_course_serializer(Profile *profiles, char *course);

// Serialization of the profiles filtred by the skills 
// Returns the profiles in json format
// Returns -1 if error occurred
char* profiles_by_skill_serializer(Profile *profiles, char *skills);

// Serialization of the profiles filtred by the year of degree 
// Returns the profiles in json format
// Returns -1 if error occurred 
char* profiles_by_year_of_degree_serializer(Profile *profiles, int year_of_degree);

// Serialization of all profiles in the database
// Returns the profiles in json format
// Returns -1 if error occurred  
char* profiles_serializer(Profile *profiles);

// Serialization of one profile filteres by e-mail
// Returns the profiles in json format
// Returns -1 if error occurred 
char* profile_by_email_serializer(Profile *profile, char *email);

#endif 