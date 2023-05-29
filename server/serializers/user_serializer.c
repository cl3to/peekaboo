#include "user_serializer.h"

// Length of the error response
#define BUFFER_SIZE 512

// Build  a admin response in json format
response_stream *_make_error_response(StatusCode status_code, int is_image)
{
  char error_msg[BUFFER_SIZE];

  sprintf(error_msg, "{\"status\":%d}", status_code);
  response_stream *error_response = (response_stream *)malloc(sizeof(response_stream));
  error_response->next = NULL;

  if (is_image)
  {
    // Build the header error message
    uint8_t header[IMAGE_HEADER_SIZE];
    // Build the header error message
    // The current message number
    header[0] = 1;
    // The total of messages
    header[1] = 1;
    // Put zeros in the image info bits because there is no imagem because the error
    header[2] = 0;
    header[3] = 0;
    header[4] = 0;
    header[5] = 0;
    header[6] = 0;
    header[7] = 0;

    // Add the header to the error_response
    memcpy(error_response->data, header, IMAGE_HEADER_SIZE);
    // Copy the error_response content after the header
    memcpy(error_response->data + IMAGE_HEADER_SIZE, error_msg, strlen(error_msg));
  }
  else
  {
    error_response->data = strdup(error_msg);
  }
  return error_response;
}

// Serialization of the profiles filtred by the academic degree
response_stream *profiles_by_course_serializer(Profile *profiles, char *course)
{
  int pid = getpid();

  if (course == NULL || strlen(course) == 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Invalid academic degree, please provide a valid input.\n", pid);
  }

  int profiles_amount = get_profiles_by_course(profiles, course);

  if (profiles_amount < 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Failed to retrieve data.\n", pid);
  }

  response_stream *user_response = (response_stream *)malloc(sizeof(response_stream));
  user_response->data = make_user_response_msg(profiles, profiles_amount, LIST_BY_COURSE);
  user_response->next = NULL;

  return user_response;
}

// Serialization of the profiles filtred by the skills
response_stream *profiles_by_skill_serializer(Profile *profiles, char *skill)
{
  int pid = getpid();

  if (skill == NULL || strlen(skill) == 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Invalid skill, please provide a valid input.\n", pid);
  }

  int profiles_amount = get_profiles_by_skill(profiles, skill);

  if (profiles_amount < 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Failed to retrieve data.\n", pid);
  }

  response_stream *user_response = (response_stream *)malloc(sizeof(response_stream));
  user_response->data = make_user_response_msg(profiles, profiles_amount, LIST_BY_SKILL);
  user_response->next = NULL;

  return user_response;
}

// Serialization of the profiles filtred by the year of degree
response_stream *profiles_by_year_of_degree_serializer(Profile *profiles, int year_of_degree)
{
  int pid = getpid();

  if (year_of_degree <= 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Invalid year of degree degree, please provide a valid input.\n", pid);
  }

  int profiles_amount = get_profiles_by_year_of_degree(profiles, year_of_degree);

  if (profiles_amount < 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Failed to retrieve data.\n", pid);
  }

  response_stream *user_response = (response_stream *)malloc(sizeof(response_stream));
  user_response->data = make_user_response_msg(profiles, profiles_amount, LIST_BY_YEAR);
  user_response->next = NULL;

  return user_response;
}

// Serialization of all profiles in the database
response_stream *profiles_serializer(Profile *profiles)
{
  int profiles_amount = get_profiles(profiles);

  if (profiles_amount < 0)
  {
    int pid = getpid();
    fprintf(stderr, "(pid %d) SERVER >>> Failed to retrieve data.\n", pid);
  }

  response_stream *user_response = (response_stream *)malloc(sizeof(response_stream));
  user_response->data = make_user_response_msg(profiles, profiles_amount, LIST_ALL_PROFILES);
  user_response->next = NULL;

  return user_response;
}

// Serialization of one profile filteres by e-mail
response_stream *profile_by_email_serializer(Profile *profile, char *email)
{
  // Process ID for prints log of the server
  int pid = getpid();

  if (check_email_format(email) != 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Invalid e-mail! Please, provide a valid e-mail.\n", pid);
  }

  int profiles_amount = get_profile_by_email(profile, email);

  if (profiles_amount < 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Failed to retrieve data.\n", pid);
  }

  response_stream *user_response = (response_stream *)malloc(sizeof(response_stream));
  user_response->data = make_user_response_msg(profile, profiles_amount, GET_PROFILE_BY_EMAIL);
  user_response->next = NULL;

  return user_response;
}

// Serialization of a profile image
response_stream *image_by_email(char *email)
{
  // Process ID for prints log of the server
  int pid = getpid();
  int image_size, total_messages, message_number, message_size, currentOffset, bytesRead;

  char filepath[MAX_LENGTH_IMAGE_NAME] = IMAGES_DIRECTORY;
  char header[IMAGE_HEADER_SIZE];
  unsigned char buffer[UDP_MAX_DATA_SIZE];

  // Variables to build the liked list
  response_stream *head = NULL;
  response_stream *current = NULL;

  // The file pointer variable
  FILE *fp;

  // Check the given email address
  if (check_email_format(email) != 0)
  {
    fprintf(stderr, "(pid %d) SERVER >>> Invalid e-mail! Please, provide a valid e-mail.\n", pid);
  }

  // Get the image size from the database
  image_size = get_image_size_by_email(email);

  // Define the image path
  // Send the default image if dont find the image size in the database
  if (image_size == -1)
  {
    image_size = DEFAULT_IMAGE_SIZE;
    strcat(filepath, "default.jpg");
  }
  else
  {
    strcat(filepath, email);
    strcat(filepath, IMAGE_EXTENSION);
  }

  // Calculate the number total of messages
  total_messages = image_size / UDP_MAX_DATA_SIZE;

  // Build the static part of the header
  // Second byte of the header receives the value of total_messages
  header[1] = (unsigned char)total_messages;

  // Last four bytes of the header receive the value of image_size
  header[4] = (image_size >> 24) & 0xFF;
  header[5] = (image_size >> 16) & 0xFF;
  header[6] = (image_size >> 8) & 0xFF;
  header[7] = image_size & 0xFF;

  // Open the image file
  fp = fopen(filepath, "rb");
  if (fp == NULL)
  {
    printf("(pid %d) SERVER >>> Error opening image file\n", pid);
    return _make_error_response(RECOVER_IMAGE_FAILED, 1);
  }

  // Read the image in packets of 1024 bytes and store in the response_stream struct
  for (message_number = 0; message_number < total_messages; message_number++)
  {

    // Determine the packet size for the current iteration
    message_size = (message_number == total_messages - 1) ? (image_size % UDP_MAX_DATA_SIZE) : UDP_MAX_DATA_SIZE;

    // First byte of the header receives the message_number
    header[0] = (uint8_t)message_number; // Messages count starts of zero

    // This bytes 2 and 3 of the header receive the value of message_size
    header[2] = (uint8_t)((message_size >> 8) & 0xFF);
    header[3] = (uint8_t)(message_size & 0xFF);

    // Allocate memory for the response_stream structure
    response_stream *packet = (response_stream *)malloc(sizeof(response_stream));
    packet->data = (char *)malloc(UDP_MAX_DATA_SIZE + IMAGE_HEADER_SIZE);
    packet->next = NULL;

    // Set the file position to the appropriate offset
    currentOffset = message_number * UDP_MAX_DATA_SIZE;
    fseek(fp, currentOffset, SEEK_SET);

    // Read the packet content from the image
    bytesRead = fread(buffer, 1, message_size, fp);
    if (bytesRead != message_size)
    {
      printf("(pid %d) SERVER >>> Error opening image file\n", pid);
      return _make_error_response(RECOVER_IMAGE_FAILED, 1);
    }

    // Add the header to the packet
    memcpy(packet->data, header, IMAGE_HEADER_SIZE);

    // Copy the packet content after the header
    memcpy(packet->data + IMAGE_HEADER_SIZE, buffer, message_size);

    // Add the packet to the linked list
    if (head == NULL)
    {
      head = packet;
      current = packet;
    }
    else
    {
      current->next = packet;
      current = current->next;
    }
  }

  // Close the image file
  fclose(fp);

  return head;
}

// TODO: put this in other file
void freeLinkedList(response_stream *head)
{
  response_stream *current = head;
  while (current != NULL)
  {
    response_stream *temp = current;
    current = current->next;
    free(temp->data);
    free(temp);
  }
}