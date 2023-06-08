#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/cJSON/cJSON.h"

#include "serializer.h"

Request *serialize_operation(OperationCode operation_code, int parameters_length, char *parameters)
{
    char *client_message = calloc(MAX_MESSAGE_SIZE, 1);
    char temp[strlen(parameters) + 100];
    // Put operation code in the first byte
    client_message[0] = (char) operation_code;

    // construct the data field in the json format
    sprintf(
        temp,
        "{\"params_length\": %d,"
        "\"params\": %s}",
        parameters_length, parameters
    );

    int message_size = strlen(temp) + 5;

    // Put parameters length in the next 4 bytes
    client_message[1] = (char) (message_size >> 24);
    client_message[2] = (char) (message_size >> 16);
    client_message[3] = (char) (message_size >> 8);
    client_message[4] = (char) (message_size);

    // Put data in the next bytes
    memcpy(client_message + 5, temp, message_size-5);

    Request *request = malloc(sizeof(Request));
    request->operation_code = operation_code;
    request->data = client_message;
    request->data_size = message_size;

    return request;
}

Request *serialize_lbc_operation(char *course)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"course\": \"%s\"}", course);
    Request *serialized_operation = serialize_operation(LIST_BY_COURSE, 1, parameters);
    free(parameters);
    return serialized_operation;
}

Request *serialize_lbs_operation(char *skills)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"skills\": \"%s\"}", skills);
    Request *serialized_operation = serialize_operation(LIST_BY_SKILL, 1, parameters);
    free(parameters);
    return serialized_operation;
}

Request *serialize_lby_operation(int year)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"year_of_degree\": %d}", year);
    Request *serialized_operation = serialize_operation(LIST_BY_YEAR, 1, parameters);
    free(parameters);
    return serialized_operation;
}

Request *serialize_la_operation()
{
    Request *serialized_operation = serialize_operation(LIST_ALL_PROFILES, 0, "{}");
    return serialized_operation;
}

Request *serialize_gp_operation(char *email)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"email\": \"%s\"}", email);
    Request *serialized_operation = serialize_operation(GET_PROFILE_BY_EMAIL, 1, parameters);
    free(parameters);
    return serialized_operation;
}

Request *serialize_dpi_operation(char *email)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"email\": \"%s\"}", email);
    Request *serialized_operation = serialize_operation(DOWNLOAD_PROFILE_IMAGE, 1, parameters);
    free(parameters);
    return serialized_operation;
}

Request *serialize_login_operation(char *password)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"password\": \"%s\"}", password);
    Request *serialized_operation = serialize_operation(LOGIN, 1, parameters);
    free(parameters);
    return serialized_operation;
}

Request *serialize_cp_operation(Profile *profile, char *session_token)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);

    char image_path[512];
    sprintf(image_path, "%speekaboo_%s.jpg", IMAGES_DIRECTORY, profile->email);

    if(access(image_path, F_OK) == -1)
    {
        sprintf(image_path, "%s%s.jpg", IMAGES_DIRECTORY, DEFAULT_IMAGE);
    }

    FILE *image = fopen(image_path, "rb");
    if (image == NULL)
    {
        fprintf(stderr, "Error opening image file.\n");
        return NULL;
    }

    // Get the image size
    fseek(image, 0, SEEK_END);
    int image_size = (int) ftell(image);

    // Make sure the image size is not larger than the maximum size allowed
    if (image_size > MAX_IMAGE_SIZE)
    {
        fprintf(stderr, "Image size too big.\n");
        return NULL;
    }

    char *image_buffer = malloc(image_size);

    // Copy the image content to the buffer
    fseek(image, 0, SEEK_SET);
    fread(image_buffer, 1, image_size, image);
    fclose(image);

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
        "\"skills\": \"%s\","
        "\"image\": %d"
        "}",
        session_token,
        profile->email,
        profile->name,
        profile->last_name,
        profile->city,
        profile->course,
        profile->year_of_degree,
        profile->skills,
        image_size
    );

    Request *request = serialize_operation(NEW_PROFILE, 9, parameters);

    // Concatenate image buffer to serialized operation
    char *data_with_img = calloc(MAX_MESSAGE_SIZE+image_size, 1);
    
    // Copy the data and image to the new buffer
    memcpy(data_with_img, request->data, request->data_size);
    memcpy(data_with_img + request->data_size, image_buffer, image_size);

    // Free unused buffers
    free(parameters);
    free(request->data);

    // Update the resquest data and size
    request->data = data_with_img;
    request->data_size += image_size;

    return request;
}

Request *serialize_rp_operation(char *email, char *session_token)
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
    Request *serialized_operation = serialize_operation(REMOVE_PROFILE_BY_EMAIL, 2, parameters);
    free(parameters);
    return serialized_operation;
}

Request *serialize_logout_operation(char *session_token)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(
        parameters,
        "{"
        "\"session_token\": \"%s\""
        "}",
        session_token);
    Request *serialized_operation = serialize_operation(LOGOUT, 1, parameters);
    free(parameters);
    return serialized_operation;
}

Profile *deserialize_profile(uint8_t *response, int *data_len)
{
    // Json parsing using cJSON library
    cJSON *json = cJSON_Parse((char*) response);
    if (json == NULL)
    {
        fprintf(stderr, "Error parsing JSON.\n");
        return NULL;
    }

    cJSON *data_len_item, *data_item, *profile_item;
    data_len_item = cJSON_GetObjectItemCaseSensitive(json, "data_length");

    if (
        data_len_item == NULL ||
        !cJSON_IsNumber(data_len_item))
    {
        fprintf(stderr, "Invalid response\n");
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

char *deserialize_authentication(uint8_t *response)
{
    // Json parsing using cJSON library
    cJSON *json = cJSON_Parse((char*) response);
    if (json == NULL)
    {
        fprintf(stderr, "Error parsing JSON.\n");
        return NULL;
    }

    cJSON *status_item, *session_token_item;
    status_item = cJSON_GetObjectItemCaseSensitive(json, "status");
    session_token_item = cJSON_GetObjectItemCaseSensitive(json, "session_token");

    if (
        status_item == NULL ||
        !cJSON_IsNumber(status_item))
    {
        fprintf(stderr, "Invalid response.\n");
        return NULL;
    }

    if (status_item->valueint == 401)
    {
        fprintf(stderr, "Invalid password.\n");
        return NULL;
    }
    if (status_item->valueint == 400)
    {
        fprintf(stderr, "Authentication Fail.\n");
        return NULL;
    }

    return session_token_item->valuestring;
}

char *deserialize_admin_operation_response(uint8_t *response, int *status_p)
{
    // Json parsing using cJSON library
    cJSON *json = cJSON_Parse((char*) response);
    if (json == NULL)
    {
        fprintf(stderr, "Error parsing JSON.\n");
        return NULL;
    }

    cJSON *status_item;
    status_item = cJSON_GetObjectItemCaseSensitive(json, "status");

    if (status_item == NULL || !cJSON_IsNumber(status_item))
    {
        fprintf(stderr, "Invalid response.\n");
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