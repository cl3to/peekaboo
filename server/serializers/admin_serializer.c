#include "admin_serializer.h"

// Length of the admin resposne
#define BUFFER_SIZE 512

// Pointer to the global variable with the session token
char *expected_session_token;

// Generate a token to the current serssion using the valid_chars
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

// Validate the given session token
int validate_session_token(char *session_token)
{
  if (session_token != NULL && strcmp(session_token, expected_session_token) == 0)
  {
    return 0;
  }
  int pid = getpid();
  fprintf(stderr, "(pid %d) SERVER >>> Invalid session token.\n", pid);
  return -1;
}

// Validate the given password
char *validate_password(char *password)
{
  int pid = getpid();
  if (password == NULL)
    return NULL;

  // Open file with the password
  FILE *pwd_file;
  pwd_file = fopen("../password_sha1.pwd", "rb");

  if (pwd_file == NULL)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Cannot open password file. \n", pid);
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
    fprintf(stderr, "(pid %d) SERVER >>> Error reading password file.\n", pid);
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
    fprintf(stderr, "(pid %d) SERVER >>> An error occurred while creating the password hash.\n", pid);
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
  fprintf(stderr, "(pid %d) SERVER >>> The password provided is invalid.\n", pid);
  return NULL;
}

// Build  a admin response in json format
response_stream *_make_admin_response(StatusCode status_code, char *session_token)
{
  char response_msg[BUFFER_SIZE];
  if (session_token == NULL)
  {
    sprintf(
        response_msg,
        "{\"status\":%d}",
        status_code);
  }
  else
  {
    sprintf(
        response_msg,
        "{\"status\":%d,\"session_token\":\"%s\"}",
        status_code,
        session_token);
  }

  response_stream *admin_response = (response_stream *)malloc(sizeof(response_stream));
  admin_response->data = strdup(response_msg);
  admin_response->next = NULL;

  return admin_response;
}

// Login to the system's administrative area
response_stream *login_with_password(char *password)
{
  char *new_session_token = validate_password(password);

  if (new_session_token == NULL)
  {
    return _make_admin_response(INVALID_PASSWORD, NULL);
  }

  return _make_admin_response(SUCCESS, new_session_token);
}

// Logout from the system's administrative area
response_stream *logout(char *session_token)
{
  if (validate_session_token(session_token) == 0)
  {
    memset(expected_session_token, '\0', SESSION_TOKEN_LENGTH);
    return _make_admin_response(SUCCESS, NULL);
  }
  return _make_admin_response(INVALID_SESSION_TOKEN, NULL);
}

// Save a given image in bin/images using email as name
// Return 1 for success
// Return -1 for failure
int _save_profile_image(char *email, char *image, int image_size)
{
  // Process ID for prints log of the server
  int pid = getpid();

  // Define the image path and name
  char filepath[MAX_LENGTH_IMAGE_NAME] = IMAGES_DIRECTORY;
  strcat(filepath, email);
  strcat(filepath, IMAGE_EXTENSION);

  // Create or uptade the image file
  FILE *fp_new = fopen(filepath, "wb");
  if (fp_new == NULL)
  {
    printf("(pid %d) SERVER >>> Error opening file for save imagem\n", pid);
    return -1;
  }

  // write image data to the file
  int bytes_written = fwrite(image, sizeof(char), image_size, fp_new);
  if (bytes_written != image_size)
  {
    printf("(pid %d) SERVER >>> Error writing image data to file\n", pid);
    return -1;
  }

  // close file
  fclose(fp_new);

  return 1;
}

// Create a new profile
response_stream *create_new_profile(char *session_token, char *email, char *name, char *last_name, char *city, char *course, int year_of_degree, char *skills, char *image, int image_size)
{
  // Check given perfil parameters
  if (
      name == NULL ||
      last_name == NULL ||
      city == NULL ||
      course == NULL ||
      year_of_degree <= 0 ||
      skills == NULL ||
      image == NULL ||
      image_size <= 0 ||
      check_email_format(email) != 0)
  {
    return _make_admin_response(REGISTRATION_FAILED, NULL);
  }

  if (validate_session_token(session_token) == 0)
  {
    Profile *profile = new_profile(
        email, name, last_name, city, course, year_of_degree, skills, image_size);

    // Sabe the profile data and image and check that it was successfully
    if (profile == NULL || store_profile(profile) < 0 || _save_profile_image(email, image, image_size) < 0)
    {
      return _make_admin_response(REGISTRATION_FAILED, NULL);
    }
    return _make_admin_response(SUCCESS, NULL);
  }
  return _make_admin_response(INVALID_SESSION_TOKEN, NULL);
}

// Remove a profile filter from email
response_stream *remove_profile_by_email(char *session_token, char *email)
{
  // Check given email address
  if (check_email_format(email) != 0)
  {
    int pid = getpid();
    fprintf(stderr, "(pid %d) SERVER >>> Invalid e-mail! Please, provide a valid e-mail.\n", pid);
    return _make_admin_response(REMOVAL_FAILED, NULL);
  }

  if (validate_session_token(session_token) == 0)
  {

    if (delete_profile_by_email(email) == 0)
    {
      return _make_admin_response(SUCCESS, NULL);
    }
    return _make_admin_response(REMOVAL_FAILED, NULL);
  }
  return _make_admin_response(INVALID_SESSION_TOKEN, NULL);
}
