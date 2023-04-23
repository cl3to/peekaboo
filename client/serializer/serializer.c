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

char *serialize_lbs_operation(char *skill)
{
    char *parameters = malloc(MAX_PARAMETERS_SIZE);
    sprintf(parameters, "{\"skills\": \"%s\"}", skill);
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