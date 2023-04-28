#include "admin_serializer.h"

#define BUFFER_SIZE 512

char *expected_session_token;

int generate_session_token()
{
  char new_session_token[SESSION_TOKEN_LENGTH + 1];
  static char valid_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~+/#$@&*?!";

  // Generate a new session token
  srand(time(NULL));
  for (int c = 0; c < SESSION_TOKEN_LENGTH; c++)
  {
    int key = rand() % (int)(sizeof(valid_chars) - 1);
    new_session_token[c] = valid_chars[key];
  }

  new_session_token[SESSION_TOKEN_LENGTH] = '\0';

  // Uptade expected_session_token (global variable) with the new_session_token
  strncpy(expected_session_token, new_session_token, SESSION_TOKEN_LENGTH + 1);

  return 1;
}

int validate_session_token(char *session_token)
{
  if (session_token != NULL && strcmp(session_token, expected_session_token) == 0)
  {
    return 0;
  }
  printf("Invalid session token.\n");
  return -1;
}

char *validate_password(char *password)
{

  if (password == NULL)
    return NULL;

  // Open file with the password
  FILE *pwd_file;
  pwd_file = fopen("../password_sha1.pwd", "rb");

  if (pwd_file == NULL)
  {
    printf("Cannot open password file. \n");
    return NULL;
  }

  // Read password hash from file
  unsigned int expected_hash[5];
  if (fscanf(pwd_file, "%08x%08x%08x%08x%08x",
             &expected_hash[0],
             &expected_hash[1],
             &expected_hash[2],
             &expected_hash[3],
             &expected_hash[4]) != 5)
  {
    printf("Error reading password file.\n");
    fclose(pwd_file);
    return NULL;
  }

  fclose(pwd_file);

  // Calculate SHA-1 hash of given password
  SHA1Context pwd_hash;
  SHA1Reset(&pwd_hash);
  SHA1Input(&pwd_hash, (const unsigned char *)password, strlen(password));

  if (!SHA1Result(&pwd_hash))
  {
    fprintf(stderr, "An error occurred while creating the password hash.\n");
    return NULL;
  }

  // Compare generated hash with expected hash
  if (pwd_hash.Message_Digest[0] == expected_hash[0] &&
      pwd_hash.Message_Digest[1] == expected_hash[1] &&
      pwd_hash.Message_Digest[2] == expected_hash[2] &&
      pwd_hash.Message_Digest[3] == expected_hash[3] &&
      pwd_hash.Message_Digest[4] == expected_hash[4])
  {
    generate_session_token();
    return strdup(expected_session_token);
  }
  printf("The password provided is invalid.\n");
  return NULL;
}

char *make_admin_response(StatusCode status_code, char *session_token)
{
  char admin_response[BUFFER_SIZE];
  if (session_token == NULL)
  {
    sprintf(
        admin_response,
        "{\"status\":%d}",
        status_code);
  }
  else
  {
    sprintf(
        admin_response,
        "{\"status\":%d,\"session_token\":\"%s\"}",
        status_code,
        session_token);
  }
  return strdup(admin_response);
}

char *login_with_password(char *password)
{
  char *new_session_token = validate_password(password);

  if (new_session_token == NULL)
  {
    return make_admin_response(INVALID_PASSWORD, NULL);
  }

  return make_admin_response(SUCCESS, new_session_token);
}

char *logout(char *session_token)
{
  if (validate_session_token(session_token) == 0)
  {
    memset(expected_session_token, '\0', SESSION_TOKEN_LENGTH);
    return make_admin_response(SUCCESS, NULL);
  }
  return make_admin_response(INVALID_SESSION_TOKEN, NULL);
}

char *create_new_profile(char *session_token, char *email, char *name, char *last_name, char *city, char *course, int year_of_degree, char *skills)
{
  // Check given perfil parameters
  if (
      name == NULL ||
      last_name == NULL ||
      city == NULL ||
      course == NULL ||
      year_of_degree <= 0 ||
      skills == NULL ||
      check_email_format(email) != 0)
  {
    return make_admin_response(REGISTRATION_FAILED, NULL);
  }
  if (validate_session_token(session_token) == 0)
  {

    Profile *profile = new_profile(
        email, name, last_name, city, course, year_of_degree, skills);

    if (profile == NULL || store_profile(profile) < 0)
    {
      return make_admin_response(REGISTRATION_FAILED, NULL);
    }
    return make_admin_response(SUCCESS, NULL);
  }
  return make_admin_response(INVALID_SESSION_TOKEN, NULL);
}

char *remove_profile_by_email(char *session_token, char *email)
{
  if (check_email_format(email) != 0)
  {
    printf("Invalid e-mail! Please, provide a valid e-mail.\n");
    return make_admin_response(REMOVAL_FAILED, NULL);
  }

  if (validate_session_token(session_token) == 0)
  {

    if (delete_profile_by_email(email) == 0)
    {
      return make_admin_response(SUCCESS, NULL);
    }
    return make_admin_response(REMOVAL_FAILED, NULL);
  }
  return make_admin_response(INVALID_SESSION_TOKEN, NULL);
}
