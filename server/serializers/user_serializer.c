#include "../model/profile.h"
#include "user_serializer.h"
#include <stdio.h>
#include <stdlib.h>

// TODO: Extract the json functions from a propietary file > utils
// TODO: Add \n at the end of a complete profile, after "}"
char* profiles_to_json(struct profile* profiles, int profiles_amount) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    // open the JSON array
    snprintf(buffer, BUFFER_SIZE, "[");

    // iterate over the profiles
    for (int i = 0; i < profiles_amount; i++) {
        // add the profile as a JSON object
        snprintf(buffer + strlen(buffer), BUFFER_SIZE - strlen(buffer),
                 "{"
                 "\"email\":\"%s\","
                 "\"name\":\"%s\","
                 "\"last_name\":\"%s\","
                 "\"city\":\"%s\","
                 "\"academic_degree\":\"%s\","
                 "\"year_of_degree\":%d,"
                 "\"habilities\":\"%s\","
                 "\"image\":\"%s\""
                 "}%s\n",
                 profiles[i].email, profiles[i].name, profiles[i].last_name,
                 profiles[i].city, profiles[i].academic_degree,
                 profiles[i].year_of_degree, profiles[i].habilities,
                 profiles[i].image, i < profiles_amount - 1 ? "," : "");

    }

    // close the JSON array
    snprintf(buffer + strlen(buffer), BUFFER_SIZE - strlen(buffer), "]\n");

    // allocate memory for the result string
    char* result = (char*) malloc(strlen(buffer) + 1);
    if (result == NULL) {
        return NULL;
    }

    // copy the result string into the allocated memory
    strncpy(result, buffer, strlen(buffer) + 1);

    return result;
}

char* simple_profiles_to_json(struct profile* profiles, int profiles_amount) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    // open the JSON array
    snprintf(buffer, BUFFER_SIZE, "[");

    // iterate over the profiles
    for (int i = 0; i < profiles_amount; i++) {
        // add the profile as a JSON object
        snprintf(buffer + strlen(buffer), BUFFER_SIZE - strlen(buffer),
                 "{"
                 "\"email\": \"%s\","
                 "\"name\": \"%s\","
                 "\"last_name\": \"%s\","
                 "}%s\n",
                 profiles[i].email, profiles[i].name, profiles[i].last_name,
                i < profiles_amount - 1 ? "," : "");

    }

    // close the JSON array
    snprintf(buffer + strlen(buffer), BUFFER_SIZE - strlen(buffer), "]\n");

    // allocate memory for the result string
    char* result = (char*) malloc(strlen(buffer) + 1);
    if (result == NULL) {
        return NULL;
    }

    // copy the result string into the allocated memory
    strncpy(result, buffer, strlen(buffer) + 1);

    return result;
}

char* academic_profiles_to_json(struct profile* profiles, int profiles_amount) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    // open the JSON array
    snprintf(buffer, BUFFER_SIZE, "[");

    // iterate over the profiles
    for (int i = 0; i < profiles_amount; i++) {
        // add the profile as a JprofilesSON object
        snprintf(buffer + strlen(buffer), BUFFER_SIZE - strlen(buffer),
                 "{"
                 "\"email\": \"%s\","
                 "\"name\": \"%s\","
                 "\"last_name\": \"%s\","
                 "\"academic_degree\": \"%s\","
                 "}%s\n",
                 profiles[i].email, profiles[i].name, profiles[i].last_name,
                 profiles[i].academic_degree, i < profiles_amount - 1 ? "," : "");

    }

    // close the JSON array
    snprintf(buffer + strlen(buffer), BUFFER_SIZE - strlen(buffer), "]\n");

    // allocate memory for the result string
    char* result = (char*) malloc(strlen(buffer) + 1);
    if (result == NULL) {
        return NULL;
    }

    // copy the result string into the allocated memory
    strncpy(result, buffer, strlen(buffer) + 1);

    return result;
}

char* profiles_by_academic_degree_serializer(Profile *profiles, char *academic_degree){

    if (academic_degree == NULL || strlen(academic_degree) == 0){
      fprintf(stderr, "Invalid academic degree, please provide a valid input.");
      return -1;
    }

    int profiles_amount = get_profiles_by_academic_degree(profiles, academic_degree);
    
    if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.");
      return -1;
    }

    char* json_profiles = simple_profiles_to_json(profiles, profiles_amount);
    printf("-->SERVER SIDE:\n %s\n", json_profiles);

    return json_profiles;
  }

char* profiles_by_habilities_serializer(Profile *profiles, char *habilities){

    if (habilities == NULL || strlen(habilities) == 0){
      fprintf(stderr, "Invalid habilities, please provide a valid input.");
      return -1;
    }

    int profiles_amount = get_profiles_by_habilities(profiles, habilities);

    if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.");
      return -1;
    }

    char* json_profiles = simple_profiles_to_json(profiles, profiles_amount);
    printf("-->SERVER SIDE:\n %s\n", json_profiles);
    
    return json_profiles;
  }

char* profiles_by_year_of_degree_serializer(Profile *profiles, int year_of_degree){
      
    if (year_of_degree <= 0){
      fprintf(stderr, "Invalid year of degree degree, please provide a valid input.");
      return -1;
    }
    
    int profiles_amount = get_profiles_by_year_of_degree(profiles, year_of_degree);

    if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.");
      return -1;
    }

    char* json_profiles = academic_profiles_to_json(profiles, profiles_amount);
    printf("-->SERVER SIDE:\n %s\n", json_profiles);

    return json_profiles;
  }

char* profiles_serializer(Profile *profiles){

  int profiles_amount = get_profiles(profiles);

  if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.");
      return -1;
    }

  char* json_profiles = profiles_to_json(profiles, profiles_amount);
  printf("-->SERVER SIDE:\n %s\n", json_profiles);

  return json_profiles;
}

char* profile_by_email_serializer(char *email){

    // TODO: Clean code
    // TODO: Add regex to email validation
  
    Profile *profiles  = (Profile *) malloc(sizeof(Profile));
    int profiles_amount = get_profile_by_email(profiles, email);

    if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.");
      return -1;
    }

    char* json_profiles = profiles_to_json(profiles, profiles_amount);
    printf("-->SERVER SIDE:\n %s\n", json_profiles);
    
    return json_profiles;
  }