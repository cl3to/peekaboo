#include <regex.h>

int is_email_valid(char *email)
{
  regex_t regex;
  int reti;
  char msgbuf[100];

  reti = regcomp(&regex, "[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,64}", 0);

  if (reti)
  {
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
  }

  reti = regexec(&regex, email, 0, NULL, 0);
  if (!reti)
  {
    printf("Valid email\n");
  }
  else if (reti == REG_NOMATCH)
  {
    printf("Invalid email\n");
  }
  else
  {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex match failed: %s\n", msgbuf);
    exit(1);
  }

  regfree(&regex);
  return 0;
}

int main()
{
  is_email_valid("banana@email.com");
}