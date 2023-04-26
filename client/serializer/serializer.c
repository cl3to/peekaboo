#include <string.h>

#include "../../lib/cJSON.h"

#include "serializer.h"

char *serialize_operation(OperationCode operation_code, int parameters_length, char *parameters)
{
    char *client_message = malloc(MAX_MESSAGE_SIZE);
    sprintf(
        client_message,
        "{\"operation_code\": %d,"
        "\"params_length\": %d,"
        "\"params\": %s}",
        operation_code, parameters_length, parameters);
    return client_message;
}

char *serialize_lbc_operation(char *course)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"course\": \"%s\"}", course);
    char *serialized_operation = serialize_operation(LIST_BY_COURSE, 1, parameters);
    free(parameters);
    return serialized_operation;
}

char *serialize_lbs_operation(char *skills)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"skills\": \"%s\"}", skills);
    char *serialized_operation = serialize_operation(LIST_BY_SKILL, 1, parameters);
    free(parameters);
    return serialized_operation;
}

char *serialize_lby_operation(int year)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"year_of_degree\": %d}", year);
    char *serialized_operation = serialize_operation(LIST_BY_YEAR, 1, parameters);
    free(parameters);
    return serialized_operation;
}

char *serialize_la_operation()
{
    char *serialized_operation = serialize_operation(LIST_ALL_PROFILES, 0, "{}");
    return serialized_operation;
}

char *serialize_gp_operation(char *email)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"email\": \"%s\"}", email);
    char *serialized_operation = serialize_operation(GET_PROFILE_BY_EMAIL, 1, parameters);
    free(parameters);
    return serialized_operation;
}

char *serialize_login_operation(char *password)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"password\": \"%s\"}", password);
    char *serialized_operation = serialize_operation(LOGIN, 1, parameters);
    free(parameters);
    return serialized_operation;
}

char *serialize_cp_operation(Profile *profile, char *session_token)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(
        parameters,
        "{"
        "\"session_token\": \"%s\","
        "\"email\": \"%s\","
        "\"name\": \"%s\","
        "\"last_name\": \"%s\","
        "\"city\": \"%s\","
        "\"course\": \"%s\","
        "\"year_of_degree\": %d,"
        "\"skills\": \"%s\""
        "}",
        session_token,
        profile->email,
        profile->name,
        profile->last_name,
        profile->city,
        profile->course,
        profile->year_of_degree,
        profile->skills);
    char *serialized_operation = serialize_operation(NEW_PROFILE, 7, parameters);
    free(parameters);
    return serialized_operation;
}

char *serialize_rp_operation(char *email, char *session_token)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(
        parameters,
        "{"
        "\"session_token\": \"%s\","
        "\"email\": \"%s\""
        "}",
        session_token,
        email);
    char *serialized_operation = serialize_operation(REMOVE_PROFILE_BY_EMAIL, 2, parameters);
    free(parameters);
    return serialized_operation;
}

char *serialize_logout_operation(char *session_token)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(
        parameters,
        "{"
        "\"session_token\": \"%s\""
        "}",
        session_token);
    char *serialized_operation = serialize_operation(LOGOUT, 1, parameters);
    free(parameters);
    return serialized_operation;
}

Profile *deserialize_profile(char *response, int *data_len)
{
    // Json parsing using cJSON library
    cJSON *json = cJSON_Parse(response);
    if (json == NULL)
    {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    cJSON *data_len_item, *data_item, *profile_item;
    data_len_item = cJSON_GetObjectItemCaseSensitive(json, "data_length");

    if (
        data_len_item == NULL ||
        !cJSON_IsNumber(data_len_item))
    {
        printf("Invalid response\n");
        return NULL;
    }

    (*data_len) = data_len_item->valueint;

    // Contain a list of profiles in json format
    data_item = cJSON_GetObjectItemCaseSensitive(json, "data");
    Profile *profiles = malloc(sizeof(Profile) * (*data_len));

    // Iterate over json profiles array and deserialize each profile
    for (int i = 0; i < (*data_len); i++)
    {
        // Get item in data array
        profile_item = cJSON_GetArrayItem(data_item, i);

        // Get values from profile_item
        cJSON *email_item = cJSON_GetObjectItemCaseSensitive(profile_item, "email");
        cJSON *first_name_item = cJSON_GetObjectItemCaseSensitive(profile_item, "name");
        cJSON *last_name_item = cJSON_GetObjectItemCaseSensitive(profile_item, "last_name");
        cJSON *city = cJSON_GetObjectItemCaseSensitive(profile_item, "city");
        cJSON *course_item = cJSON_GetObjectItemCaseSensitive(profile_item, "course");
        cJSON *year_of_degree_item = cJSON_GetObjectItemCaseSensitive(profile_item, "year_of_degree");
        cJSON *skills_item = cJSON_GetObjectItemCaseSensitive(profile_item, "skills");

        // Set values
        if (city)
            strcpy(profiles[i].city, city->valuestring);
        if (course_item)
            strcpy(profiles[i].course, course_item->valuestring);
        if (year_of_degree_item)
            profiles[i].year_of_degree = year_of_degree_item->valueint;
        if (skills_item)
            strcpy(profiles[i].skills, skills_item->valuestring);

        strcpy(profiles[i].email, email_item->valuestring);
        strcpy(profiles[i].name, first_name_item->valuestring);
        strcpy(profiles[i].last_name, last_name_item->valuestring);
    }

    return profiles;
}

char *deserialize_authentication(char *response)
{
    // Json parsing using cJSON library
    cJSON *json = cJSON_Parse(response);
    if (json == NULL)
    {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    cJSON *status_item, *session_token_item;
    status_item = cJSON_GetObjectItemCaseSensitive(json, "status");
    session_token_item = cJSON_GetObjectItemCaseSensitive(json, "session_token");

    if (
        status_item == NULL ||
        !cJSON_IsNumber(status_item))
    {
        printf("Invalid response.\n");
        return NULL;
    }

    if (status_item->valueint == 401)
    {
        printf("Invalid password.\n");
        return NULL;
    }
    if (status_item->valueint == 400)
    {
        printf("Authentication Fail.\n");
        return NULL;
    }

    return session_token_item->valuestring;
}

char *deserialize_admin_operation_response(char *response, int *status_p)
{
    // Json parsing using cJSON library
    cJSON *json = cJSON_Parse(response);
    if (json == NULL)
    {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    cJSON *status_item;
    status_item = cJSON_GetObjectItemCaseSensitive(json, "status");

    if (status_item == NULL || !cJSON_IsNumber(status_item))
    {
        printf("Invalid response.\n");
        return NULL;
    }

    int status_code = status_item->valueint;
    int message_size = 512;
    char *status_message = malloc(message_size);

    switch (status_code)
    {
    case FAILURE:
        snprintf(status_message, message_size, "Status: %d. Server error.", status_code);
        break;
    case INVALID_SESSION_TOKEN:
        snprintf(status_message, message_size, "Status: %d. Invalid Session Token.", status_code);
        break;
    case REGISTRATION_FAILED:
        snprintf(status_message, message_size, "Status: %d. Error registering profile.", status_code);
        break;
    case REMOVAL_FAILED:
        snprintf(status_message, message_size, "Status: %d. Error removing profile.", status_code);
        break;
    case SUCCESS:
        snprintf(status_message, message_size, "Status: %d. Operation successful.", status_code);
        break;
    default:
        snprintf(status_message, message_size, "Some bananas are sweeter than others (:.");
        break;
    }

    // Save status code in pointer address to be used in caller function
    (*status_p) = status_code;
    return status_message;
}