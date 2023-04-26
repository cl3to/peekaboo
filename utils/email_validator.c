#include "email_validator.h"

int check_email_format(char *email)
{

  if (email == NULL)
    return -1;

  regex_t regex;
  int is_valid;

  // Creation of email regex
  is_valid = regcomp(&regex, "[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,64}", REG_EXTENDED);

  if (is_valid != 0)
  {
    printf("Regex compilation failed\n");
    return -1;
  }

  // Comparing pattern with email string
  is_valid = regexec(&regex, email, 0, NULL, 0);

  regfree(&regex);

  return is_valid;
}
