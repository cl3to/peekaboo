#include "general_serializer.h"

// Serializer to make json parsing and choice the correct serializer function
char *general_serializer(Profile *profiles, char *request_body)
{
  int pid = getpid();

  int operation_code, params_length;
  char *response;

  // Json parsing using cJSON library
  cJSON *json = cJSON_Parse(request_body);
  if (json == NULL)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Error parsing JSON: %s\n", pid, cJSON_GetErrorPtr());
    return NULL;
  }

  // Get the values of operation_code, params_length and params
  cJSON *operation_code_item = cJSON_GetObjectItemCaseSensitive(json, "operation_code");
  cJSON *params_length_item = cJSON_GetObjectItemCaseSensitive(json, "params_length");
  cJSON *params_items = cJSON_GetObjectItemCaseSensitive(json, "params");

  if (
      operation_code_item == NULL ||
      params_length_item == NULL ||
      params_items == NULL ||
      !cJSON_IsNumber(operation_code_item) ||
      !cJSON_IsNumber(params_length_item))
  {
    fprintf(stderr, "(pid %d) SERVER >>> Invalid request\n", pid);
    return NULL;
  }

  operation_code = operation_code_item->valueint;
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
  // TODO: Implement this for the second assignment
  case DOWNLOAD_PROFILE_IMAGE:
    fprintf(stderr, "(pid %d) SERVER >>> Wait for Peekaboo 2.0 release.\n", pid);
    response = NULL;
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
        cJSON_GetObjectItemCaseSensitive(params_items, "skills")->valuestring);
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
