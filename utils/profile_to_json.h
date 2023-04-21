#ifndef PROFILE_TO_JSON_H
#define PROFILE_TO_JSON_H

#include "../server/model/profile.h"

#include "operation_codes.h"
#include <stdlib.h>

char* make_complete_profile(struct profile* profile);

char* profiles_to_json(struct profile* profiles, int profiles_amount, OperationCode operation_code);

#endif 