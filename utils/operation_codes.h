// Enumeration of possible actions that can be performed by the user.
typedef enum {
    // Used to list all people (email and name) who have graduated in a certain course.
    LIST_BY_COURSE = 1, 
    // Used to list all people (email and name) who have a certain skill.
    LIST_BY_SKILL, 
    // Used to list all people (email, name, and course) who have graduated in a certain year.
    LIST_BY_YEAR, 
    // Used to list all the information of all profiles.
    LIST_ALL_PROFILES, 
    // Used to retrieve a profile's information based on its email.
    GET_PROFILE_BY_EMAIL, 
    // Used to download a profile's image based on its identifier (UDP only).
    DOWNLOAD_PROFILE_IMAGE, 
    // Used to remove a profile based on its email (admin only).
    REMOVE_PROFILE_BY_EMAIL, 
    // Used to create a new profile using an email as an identifier (admin only).
    NEW_PROFILE, 
    // Used to indicate that the user wishes to log in (admin only).
    LOGIN, 
    // Used to indicate that the user wishes to exit the program (admin only).
    LOGOUT0 
} OperationCode;



