#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "../../model/profile.h"
#include "../../utils/constants.h"

#define MAX_MESSAGE_SIZE 2048
#define MAX_PARAMETERS_SIZE 2048

// Serialize the peekaboo operation
// Returns the serialized operation
Request *serialize_operation(OperationCode operation_code, int parameters_length, char *parameters);

// Create List by Course (LBC) operation
// Returns the serialized operation
Request *serialize_lbc_operation(char *course);

// Create List by Skill (LBS) operation
// Returns the serialized operation
Request *serialize_lbs_operation(char *skills);

// Create List by Year (LBY) operation
// Returns the serialized operation
Request *serialize_lby_operation(int year);

// Create List All (LA) operation
// Returns the serialized operation
Request *serialize_la_operation();

// Create Get Profile (GP) operation
// Returns the serialized operation
Request *serialize_gp_operation(char *email);

// Create Download Profile Image (DPI) operation
// Returns the serialized operation
Request *serialize_dpi_operation(char *email);

// Create a Login operation
// Returns the serialized operation
Request *serialize_login_operation(char *password);

// Create a Create Profile (CP) operation
// Returns the serialized operation
Request *serialize_cp_operation(Profile *profile, char* session_token);

// Create a Remove Profile (RP) operation
// Returns the serialized operation
Request *serialize_rp_operation(char *email, char *session_token);

// Create a Logout operation
// Returns the serialized operation
Request *serialize_logout_operation(char *session_token);

// Transform the server response into a Profile List
// Returns the Profile List
Profile *deserialize_profile(char *response, int *data_len);

// Transform the server response into a Authentication data
// Returns the Authentication data
char *deserialize_authentication(char *response);

// Check if the server response is a success
// Returns a message indicating if the operation was successful or not
// if the operation was failed, returns the error message
char *deserialize_admin_operation_response(char *response, int *status_p);

#endif