#ifndef PROFILE_TO_JSON_H
#define PROFILE_TO_JSON_H

#include <stdlib.h>

#include "../model/profile.h"
#include "constants.h"

char *make_complete_profile(struct profile *profile);

char *profiles_to_json(struct profile *profiles, int profiles_amount, OperationCode operation_code);

char* make_empty_response(OperationCode operation_code, char* buffer);

#endif