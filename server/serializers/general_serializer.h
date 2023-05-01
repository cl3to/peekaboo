#ifndef GENERAL_SERIALIZER_H
#define GENERAL_SERIALIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../lib/cJSON/cJSON.h"

#include "admin_serializer.h"
#include "user_serializer.h"

// Serializer to make json parsing and choice the correct serializer function
// Returns a response with a json string
char *general_serializer(Profile *profiles, char *request_body);

#endif