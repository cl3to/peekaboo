#include "profile.h"

// Create a new profile
Profile *new_profile(char *email, char *name, char *last_name, char *city, char *course, int year_of_degree, char *skills, int image_size)
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
    profile->image = image_size;
    return profile;
}

// Free the response_stream liked list
void freeLinkedList(response_stream *head)
{
  response_stream *current = head;
  while (current != NULL)
  {
    response_stream *temp = current;
    current = current->next;
    free(temp->data);
    free(temp);
  }
}