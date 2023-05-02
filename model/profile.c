#include "profile.h"

// Create a new profile
Profile *new_profile(char *email, char *name, char *last_name, char *city, char *course, int year_of_degree, char *skills)
{
    Profile *profile = (Profile *)malloc(sizeof(Profile));
    if (profile == NULL)
    {
        return NULL;
    }
    strcpy(profile->email, email);
    strcpy(profile->name, name);
    strcpy(profile->last_name, last_name);
    strcpy(profile->city, city);
    strcpy(profile->course, course);
    profile->year_of_degree = year_of_degree;
    strcpy(profile->skills, skills);
    // TODO: set image for the second assignment
    profile->image[0] = '\0';
    return profile;
}