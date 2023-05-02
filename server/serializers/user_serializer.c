#include "user_serializer.h"

// Serialization of the profiles filtred by the academic degree
char *profiles_by_course_serializer(Profile *profiles, char *course)
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

  char *user_response = make_user_response_msg(profiles, profiles_amount, LIST_BY_COURSE);

  return user_response;
}

// Serialization of the profiles filtred by the skills
char *profiles_by_skill_serializer(Profile *profiles, char *skill)
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

  char *user_response = make_user_response_msg(profiles, profiles_amount, LIST_BY_SKILL);

  return user_response;
}

// Serialization of the profiles filtred by the year of degree
char *profiles_by_year_of_degree_serializer(Profile *profiles, int year_of_degree)
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

  char *user_response = make_user_response_msg(profiles, profiles_amount, LIST_BY_YEAR);

  return user_response;
}

// Serialization of all profiles in the database
char *profiles_serializer(Profile *profiles)
{
  int profiles_amount = get_profiles(profiles);

  if (profiles_amount < 0)
  {
    int pid = getpid();
    fprintf(stderr, "(pid %d) SERVER >>> Failed to retrieve data.\n", pid);
  }

  char *user_response = make_user_response_msg(profiles, profiles_amount, LIST_ALL_PROFILES);

  return user_response;
}

// Serialization of one profile filteres by e-mail
char *profile_by_email_serializer(Profile *profile, char *email)
{
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

  char *user_response = make_user_response_msg(profile, profiles_amount, GET_PROFILE_BY_EMAIL);

  return user_response;
}