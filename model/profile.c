#include "profile.h"

// Create a new profile
Profile *new_profile(char *email, char *name, char *last_name, char *city, char *academic_degree, int year_of_degree, char *habilities) {
    Profile *profile = (Profile *) malloc(sizeof(Profile));
    if (profile == NULL) {
        return NULL;
    }
    strcpy(profile->email, email);
    strcpy(profile->name, name);
    strcpy(profile->last_name, last_name);
    strcpy(profile->city, city);
    strcpy(profile->academic_degree, academic_degree);
    profile->year_of_degree = year_of_degree;
    strcpy(profile->habilities, habilities);
    // image is not set
    profile->image[0] = '\0';
    return profile;
}