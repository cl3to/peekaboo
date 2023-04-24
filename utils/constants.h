#ifndef CONSTANTS_H
#define CONSTANTS_H

// Enumeration of possible actions that can be performed by the user.
typedef enum OperationCode
{
    // 0- Used to indicate that the user wishes to exit the program.
    EXIT = 0,
    // USER ACTIONS
    // 1- Used to list all people (email and name) who have graduated in a certain course.
    LIST_BY_COURSE = 1,
    // 2- Used to list all people (email and name) who have a certain skill.
    LIST_BY_SKILL,
    // 3- Used to list all people (email, name, and course) who have graduated in a certain year.
    LIST_BY_YEAR,
    // 4- Used to list all the information of all profiles.
    LIST_ALL_PROFILES,
    // 5- Used to retrieve a profile's information based on its email.
    GET_PROFILE_BY_EMAIL,
    // 6- Used to download a profile's image based on its identifier (UDP only).
    DOWNLOAD_PROFILE_IMAGE,

    // ADMIN ACTIONS
    // 7- Used to remove a profile based on its email (admin only).
    REMOVE_PROFILE_BY_EMAIL,
    // 8- Used to create a new profile using an email as an identifier (admin only).
    NEW_PROFILE,
    // 9- Used to indicate that the user wishes to log in (admin only).
    LOGIN,
    // 10- Used to indicate that the user wishes to exit the program (admin only).
    LOGOUT
} OperationCode;

// Enumeration of possible status of the server response.
typedef enum StatusCode
{
    // Status code for successful action on the server
    SUCCESS = 200,
    // Status code for failure action on the server
    FAILURE = 400
} StatusCode;

// Some constants
#define MAXDATASIZE 64 * 1024   // max number of bytes we can get at once
#define PORT "4527"             // Peekaboo port number
#define SESSION_TOKEN_LENGTH 42 // Session token length

#endif