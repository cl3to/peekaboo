#ifndef PROFILES_USER_H
#define PROFILES_USER_H

#include <stdlib.h>
#include <string.h>

// Serialization of the profiles filtred by the academic degree 
// Returns the profiles in json format
// Returns -1 if error occurred
char* profiles_by_academic_degree_serializer(Profile *profiles, char *academic_degree);

// Serialization of the profiles filtred by the habilities 
// Returns the profiles in json format
// Returns -1 if error occurred
char* profiles_by_habilities_serializer(Profile *profiles, char *habilities);

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