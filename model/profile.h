#ifndef PROFILES_H
#define PROFILES_H

#include <stdlib.h>
#include <string.h>

struct profile {
    char email[100];
    char name[65];
    char last_name[65];
    char city[100]; // city of residence
    char academic_degree[100];
    int year_of_degree;
    char habilities[300]; // comma separated list of habilities
    char image[200]; // path to the image
};

typedef struct profile Profile;

// Create a new profile
// Returns a pointer to the new profile
Profile *new_profile(char *email, char *name, char *last_name, char *city, char *academic_degree, int year_of_degree, char *habilities);

#endif