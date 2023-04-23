#include "general_serializer.h"

char *general_serializer(Profile *profiles, char *request_body)
{

  int operation_code, params_length;
  char *response;

  // Json parsing using cJSON library
  cJSON *json = cJSON_Parse(request_body);
  if (json == NULL)
  {
    printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
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
    printf("Invalid request\n");
    return NULL;
  }

  operation_code = operation_code_item->valueint;
  params_length = params_length_item->valueint;

  // Choise the right serializer for the operation
  switch (operation_code)
  {
  case LIST_BY_COURSE:
    response = profiles_by_academic_degree_serializer(
        profiles,
        cJSON_GetObjectItemCaseSensitive(params_items, "academic_degree")->valuestring);
    break;
  case LIST_BY_SKILL:
    response = profiles_by_habilities_serializer(
        profiles,
        cJSON_GetObjectItemCaseSensitive(params_items, "habilities")->valuestring);
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
    printf("Wait for Peekaboo 2.0 release.\n");
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
        cJSON_GetObjectItemCaseSensitive(params_items, "academic_degree")->valuestring,
        cJSON_GetObjectItemCaseSensitive(params_items, "year_of_degree")->valueint,
        cJSON_GetObjectItemCaseSensitive(params_items, "habilities")->valuestring);
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
    printf("Unknown operation! Please check your request.");
    break;
  }

  // Free the memory allocated for the JSON
  cJSON_Delete(json);

  return response;
}
