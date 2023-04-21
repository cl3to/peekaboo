#ifndef PROFILES_H
#define PROFILES_H

#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

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


// Store the profile in the database using sqlite3
// Returns 0 if the profile was stored successfully
// Returns -1 if the profile was not stored
int store_profile(Profile *profile);


// Get all the profiles by academic degree from the database using sqlite3
// The information retrived is the name, last name and email
// Returns the number of profiles were retrieved successfully
// Returns -1 if the profiles were not retrieved
int get_profiles_by_academic_degree(Profile *profiles, char *academic_degree);


// List all the profiles from the database that have the habilities
// The information retrived is the name, last name and email
// Returns the number of profiles were retrieved successfully
// Returns -1 if the profiles were not retrieved
int get_profiles_by_habilities(Profile *profiles, char *habilities);


// List all the profiles from the database that have the year of degree
// The information retrived is the name, last name , email and academic degree
// Returns the number of profiles were retrieved successfully
// Returns -1 if the profiles were not retrieved
int get_profiles_by_year_of_degree(Profile *profiles, int year_of_degree);


// List all profiles from the database
// Returns the number of profiles were retrieved successfully
// Returns -1 if the profiles were not retrieved
int get_profiles(Profile *profiles);


// Get the profile from the database using sqlite3
// Returns the number of profiles were retrieved successfully
// Returns -1 if the profile was not retrieved
int get_profile_by_email(Profile *profile, char *email);


// Delete the profile from the database that have the email
// Returns 0 if the profile was deleted successfully
// Returns -1 if the profile was not deleted
int delete_profile_by_email(char *email);

#endif