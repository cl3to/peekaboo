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

Profile *deserialize_profile(char *response, int *data_len)
{
    // Json parsing using cJSON library
    cJSON *json = cJSON_Parse(response);
    if (json == NULL)
    {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return NULL;
    }

    cJSON *op_code_item, *data_len_item, *data_item, *profile_item;
    op_code_item = cJSON_GetObjectItemCaseSensitive(json, "operation_code");
    data_len_item = cJSON_GetObjectItemCaseSensitive(json, "data_length");

    if (
        op_code_item == NULL ||
        data_len_item == NULL ||
        !cJSON_IsNumber(op_code_item) ||
        !cJSON_IsNumber(data_len_item))
    {
        printf("Invalid response\n");
        return NULL;
    }

    // OperationCode op_code = (OperationCode) op_code_item->valueint;
    (*data_len) = data_len_item->valueint;

    data_item = cJSON_GetObjectItemCaseSensitive(json, "data");
    
    Profile *profiles = malloc(sizeof(Profile) * (*data_len));

    for(int i = 0; i < (*data_len); i++)
    {
        // Get item in data array
        profile_item = cJSON_GetArrayItem(data_item, i);

        // Get values from profile_item
        cJSON *email_item = cJSON_GetObjectItemCaseSensitive(profile_item, "email");
        cJSON *first_name_item = cJSON_GetObjectItemCaseSensitive(profile_item, "name");
        cJSON *last_name_item = cJSON_GetObjectItemCaseSensitive(profile_item, "last_name");
        cJSON *city  = cJSON_GetObjectItemCaseSensitive(profile_item, "city");
        cJSON *course_item = cJSON_GetObjectItemCaseSensitive(profile_item, "academic_degree");
        cJSON *year_of_degree_item = cJSON_GetObjectItemCaseSensitive(profile_item, "year_of_degree");
        cJSON *skills_item = cJSON_GetObjectItemCaseSensitive(profile_item, "habilities");

        // Set values
        if (city)
            strcpy(profiles[i].city, city->valuestring);
        if (course_item)
            strcpy(profiles[i].academic_degree, course_item->valuestring);
        if (year_of_degree_item)
            profiles[i].year_of_degree = year_of_degree_item->valueint;
        if (skills_item)
            strcpy(profiles[i].habilities, skills_item->valuestring);

        strcpy(profiles[i].email, email_item->valuestring);
        strcpy(profiles[i].name, first_name_item->valuestring);
        strcpy(profiles[i].last_name, last_name_item->valuestring);
    }

    return profiles;
}

