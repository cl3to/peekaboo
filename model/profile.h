#ifndef PROFILES_H
#define PROFILES_H

#include <stdlib.h>
#include <string.h>

// Struct to store profile information
struct profile
{
    char email[100];
    char name[65];
    char last_name[65];
    char city[100]; // city of residence
    char course[100];
    int year_of_degree;
    char skills[300]; // comma separated list of skills
    char image[200];  // path to the image
};

typedef struct profile Profile;

// Create a new profile
// Returns a pointer to the new profile
Profile *new_profile(char *email, char *name, char *last_name, char *city, char *course, int year_of_degree, char *skills);

#endif