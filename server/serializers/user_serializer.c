#include <stdio.h>

#include "../database/database.h"
#include "user_serializer.h"
#include "../../utils/profile_to_json.h"

char* profiles_by_academic_degree_serializer(Profile *profiles, char *academic_degree){

    if (academic_degree == NULL || strlen(academic_degree) == 0){
      fprintf(stderr, "Invalid academic degree, please provide a valid input.\n");
      return NULL;
    }

    int profiles_amount = get_profiles_by_academic_degree(profiles, academic_degree);
    
    if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.\n");
      return NULL;
    }

    char* json_profiles = profiles_to_json(profiles, profiles_amount, LIST_BY_COURSE);

    return json_profiles;
  }

char* profiles_by_habilities_serializer(Profile *profiles, char *habilities){

    if (habilities == NULL || strlen(habilities) == 0){
      fprintf(stderr, "Invalid habilities, please provide a valid input.\n");
      return NULL;
    }

    int profiles_amount = get_profiles_by_habilities(profiles, habilities);

    if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.\n");
      return NULL;
    }

    char* json_profiles = profiles_to_json(profiles, profiles_amount, LIST_BY_SKILL);
    
    return json_profiles;
  }

char* profiles_by_year_of_degree_serializer(Profile *profiles, int year_of_degree){
      
    if (year_of_degree <= 0){
      fprintf(stderr, "Invalid year of degree degree, please provide a valid input.\n");
      return NULL;
    }
    
    int profiles_amount = get_profiles_by_year_of_degree(profiles, year_of_degree);

    if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.\n");
      return NULL;
    }

    char* json_profiles = profiles_to_json(profiles, profiles_amount, LIST_BY_YEAR);

    return json_profiles;
  }

char* profiles_serializer(Profile *profiles){

  int profiles_amount = get_profiles(profiles);

  if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.\n");
      return NULL;
    }

  char* json_profiles = profiles_to_json(profiles, profiles_amount, LIST_ALL_PROFILES);

  return json_profiles;
}

char* profile_by_email_serializer(Profile *profile, char *email){

    // TODO: Add regex to email validation --> move to utils
  
    int profiles_amount = get_profile_by_email(profile, email);

    if(profiles_amount < 0){
      fprintf(stderr, "Failed to retrieve data.\n");
      return NULL;
    }

    char* json_profiles = profiles_to_json(profile, profiles_amount, GET_PROFILE_BY_EMAIL);
    
    return json_profiles;
  }