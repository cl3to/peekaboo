#include <stdlib.h>
#include <stdio.h>

#include "profile_to_json.h"

char* make_complete_profile(struct profile* profile) {
    const int LINE_BUFFER_SIZE = 1024;
    char buffer[LINE_BUFFER_SIZE];

    snprintf(buffer, LINE_BUFFER_SIZE, "{"
             "\"email\":\"%s\","
             "\"name\":\"%s\","
             "\"last_name\":\"%s\","
             "\"city\":\"%s\","
             "\"course\":\"%s\","
             "\"year_of_degree\":%d,"
             "\"skills\":\"%s\","
             "\"image\":\"%s\""
             "}",
             profile->email, profile->name, profile->last_name,
             profile->city, profile->course,
             profile->year_of_degree, profile->skills,
             profile->image);

    return strdup(buffer);
}

char* make_simple_profile(struct profile* profile) {
    const int LINE_BUFFER_SIZE = 256;
    char buffer[LINE_BUFFER_SIZE];

    snprintf(buffer, LINE_BUFFER_SIZE, "{"
             "\"email\":\"%s\","
             "\"name\":\"%s\","
             "\"last_name\":\"%s\""
             "}",
             profile->email, profile->name, profile->last_name);

    return strdup(buffer);
}

char* make_academic_profile(struct profile* profile) {
    const int LINE_BUFFER_SIZE = 256;
    char buffer[LINE_BUFFER_SIZE];

    snprintf(buffer, LINE_BUFFER_SIZE, "{"
             "\"email\":\"%s\","
             "\"name\":\"%s\","
             "\"last_name\":\"%s\","
             "\"course\":\"%s\""
             "}",
             profile->email, profile->name, profile->last_name,
             profile->course);

    return strdup(buffer);
}

char* profiles_to_json(struct profile* profiles, int profiles_amount, OperationCode operation_code) {

    char buffer[MAXDATASIZE];

    // open the JSON array
    snprintf(buffer, MAXDATASIZE, "{"
        "\"operation_code\": %d,"
        "\"data_length\": %d,"
        "\"data\": [",
        operation_code, profiles_amount
    );

    // iterate over the profiles
    for (int i = 0; i < profiles_amount; i++) {
        char* profile_json;
        switch (operation_code)
        {
        
        case LIST_ALL_PROFILES:
        case GET_PROFILE_BY_EMAIL:
            profile_json = make_complete_profile(&profiles[i]);
            break;
        
        case LIST_BY_YEAR:
            profile_json = make_academic_profile(&profiles[i]);
            break;
        
        default:
            profile_json = make_simple_profile(&profiles[i]);
            break;
        }
        
        // add the profile as a JSON object
        snprintf(buffer + strlen(buffer), MAXDATASIZE - strlen(buffer), "%s%s", profile_json, i < profiles_amount - 1 ? "," : "");

        free(profile_json);
    }

    // close the JSON array
    snprintf(buffer + strlen(buffer), MAXDATASIZE - strlen(buffer), "]}");

    return strdup(buffer);
}
