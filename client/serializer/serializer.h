#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/constants.h"

#define MAX_MESSAGE_SIZE 1024
#define MAX_PARAMETERS_SIZE 256

// Validate the email
// Returns 0 if the email is valid
int validate_email(char *email);

// Serialize the peekaboo operation
// Returns the serialized operation
char *serialize_operation(OperationCode operation_code, int parameters_length, char *parameters);

// Create List by Course (LBC) operation
// Returns the serialized operation
char *serialize_lbc_operation(char *course);

// Create List by Skill (LBS) operation
// Returns the serialized operation
char *serialize_lbs_operation(char *skill);

// Create List by Year (LBY) operation
// Returns the serialized operation
char *serialize_lby_operation(int year);

// Create List All (LA) operation
// Returns the serialized operation
char *serialize_la_operation();

// Create Get Profile (GP) operation
// Returns the serialized operation
char *serialize_gp_operation(char *email);

#endif