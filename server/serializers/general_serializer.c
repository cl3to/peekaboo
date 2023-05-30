#include "general_serializer.h"

// Serializer to make json parsing and choice the correct serializer function
response_stream *general_serializer(Profile *profiles, char *request_message)
{
  // Process ID for prints log of the server
  int pid = getpid();

  int operation_code, params_length;
  int body_size = 0;
  response_stream *response;

  // Retrieve the message metadata
  operation_code = (int)request_message[0];
  memcpy(&body_size, request_message + OP_CODE_SIZE, MESSAGE_BODY_SIZE_SIZE);
  body_size = ntohl(body_size); // Convert body_size from network byte order to host byte order

  // Retrive the message body in JSON format
  char *request_body = malloc(body_size + 1);
  memcpy(request_body, request_message + HEADER_SIZE, body_size);
  request_body[body_size] = '\0';

  // Json parsing using cJSON library
  cJSON *json = cJSON_Parse(request_body);
  if (json == NULL)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Error parsing JSON: %s\n", pid, cJSON_GetErrorPtr());
    return NULL;
  }

  // Get the values of operation_code, params_length and params
  cJSON *params_length_item = cJSON_GetObjectItemCaseSensitive(json, "params_length");
  cJSON *params_items = cJSON_GetObjectItemCaseSensitive(json, "params");

  if (
      params_length_item == NULL ||
      params_items == NULL ||
      !cJSON_IsNumber(params_length_item))
  {
    fprintf(stderr, "(pid %d) SERVER >>> Invalid request\n", pid);
    return NULL;
  }

  params_length = params_length_item->valueint;

  // Choise the right serializer for the operation
  switch (operation_code)
  {
  case LIST_BY_COURSE:
    response = profiles_by_course_serializer(
        profiles,
        cJSON_GetObjectItemCaseSensitive(params_items, "course")->valuestring);
    break;
  case LIST_BY_SKILL:
    response = profiles_by_skill_serializer(
        profiles,
        cJSON_GetObjectItemCaseSensitive(params_items, "skills")->valuestring);
    break;
  case LIST_BY_YEAR:
    response = profiles_by_year_of_degree_serializer(
        profiles,
        cJSON_GetObjectItemCaseSensitive(params_items, "year_of_degree")->valueint);
    break;
  case LIST_ALL_PROFILES:
    response = profiles_serializer(profiles);
    break;
  case GET_PROFILE_BY_EMAIL:
    response = profile_by_email_serializer(
        profiles,
        cJSON_GetObjectItemCaseSensitive(params_items, "email")->valuestring);
    break;
  case DOWNLOAD_PROFILE_IMAGE:
    response = image_by_email(
        cJSON_GetObjectItemCaseSensitive(params_items, "email")->valuestring
    );
    break;

  // ADMIN ACTIONS
  case REMOVE_PROFILE_BY_EMAIL:
    response = remove_profile_by_email(
        cJSON_GetObjectItemCaseSensitive(params_items, "session_token")->valuestring,
        cJSON_GetObjectItemCaseSensitive(params_items, "email")->valuestring);
    break;
  case NEW_PROFILE:
    response = create_new_profile(
        cJSON_GetObjectItemCaseSensitive(params_items, "session_token")->valuestring,
        cJSON_GetObjectItemCaseSensitive(params_items, "email")->valuestring,
        cJSON_GetObjectItemCaseSensitive(params_items, "name")->valuestring,
        cJSON_GetObjectItemCaseSensitive(params_items, "last_name")->valuestring,
        cJSON_GetObjectItemCaseSensitive(params_items, "city")->valuestring,
        cJSON_GetObjectItemCaseSensitive(params_items, "course")->valuestring,
        cJSON_GetObjectItemCaseSensitive(params_items, "year_of_degree")->valueint,
        cJSON_GetObjectItemCaseSensitive(params_items, "skills")->valuestring,
        request_message + body_size, // pointer to the image data
        cJSON_GetObjectItemCaseSensitive(params_items, "image")->valueint);
    break;
  case LOGIN:
    response = login_with_password(
        cJSON_GetObjectItemCaseSensitive(params_items, "password")->valuestring);
    break;
  case LOGOUT:
    response = logout(
        cJSON_GetObjectItemCaseSensitive(params_items, "session_token")->valuestring);
    break;

  default:
    fprintf(stderr, "(pid %d) SERVER >>> Unknown operation! Please check your request.", pid);
    break;
  }

  // Free the memory allocated for the JSON
  cJSON_Delete(json);

  return response;
}
