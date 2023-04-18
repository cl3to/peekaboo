#include <stdio.h>
#include "database.h"

// Open the database connection
sqlite3 *open_db() {
    sqlite3 *db;
    int rc = sqlite3_open("profiles.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

// Close the database connection
// returns 0 if the database was closed successfully
// returns 1 if the database was not closed successfully
int close_db(sqlite3 *db) {
    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    return 0;
}

// Create the profiles table if it does not exist
void create_profiles_table() {
    sqlite3 *db = open_db();
    if (db == NULL) {
        return;
    }
    char *err_msg = 0;
    int rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS profiles ("
                            "email VARCHAR(100) PRIMARY KEY, "
                            "name VARCHAR(65), "
                            "last_name VARCHAR(65), "
                            "city VARCHAR(100), "
                            "academic_degree VARCHAR(100), "
                            "year_of_degree INTEGER, "
                            "habilities VARCHAR(300), "
                            "image VARCHAR(200)"
                            ");",
                            0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    sqlite3_close(db);
}

// Create a new profile
Profile *new_profile(char *email, char *name, char *last_name, char *city, char *academic_degree, int year_of_degree, char *habilities) {
    create_profiles_table();
    Profile *profile = (Profile *) malloc(sizeof(Profile));
    if (profile == NULL) {
        return NULL;
    }
    strcpy(profile->email, email);
    strcpy(profile->name, name);
    strcpy(profile->last_name, last_name);
    strcpy(profile->city, city);
    strcpy(profile->academic_degree, academic_degree);
    profile->year_of_degree = year_of_degree;
    strcpy(profile->habilities, habilities);
    // image is not set
    profile->image[0] = '\0';
    return profile;
}


int store_profile(Profile *profile) {
    sqlite3 *db;
    char *sql;
    int rc;

    // Open the database
    db = open_db();

    // Construct the SQL statement to insert a new profile
    sql = sqlite3_mprintf(
        "INSERT INTO profiles (email, name, last_name, city, academic_degree, year_of_degree, habilities, image) "
        "VALUES ('%q', '%q', '%q', '%q', '%q', %d, '%q', '%q');",
        profile->email, profile->name, profile->last_name, profile->city, profile->academic_degree, 
        profile->year_of_degree, profile->habilities, profile->image);

    // Execute the SQL statement
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    sqlite3_free(sql);
    close_db(db);

    if (rc != SQLITE_OK) {
        return 1;
    }

    return 0;
}

int get_profiles_by_academic_degree(Profile *profiles, char *academic_degree) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    // Open the database
    rc = sqlite3_open("profiles.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Prepare the SQL statement
    char *sql = sqlite3_mprintf("SELECT email, name, last_name FROM profiles WHERE academic_degree='%q'", academic_degree);
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_free(sql);
        return 1;
    }

    // Execute the statement
    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        strcpy(profiles[i].email, sqlite3_column_text(stmt, 0));
        strcpy(profiles[i].name, sqlite3_column_text(stmt, 1));
        strcpy(profiles[i].last_name, sqlite3_column_text(stmt, 2));
        i++;
    }

    // Finalize the statement and free memory
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    // Close the database
    sqlite3_close(db);

    if (i > 0) {
        return 0; // profiles were retrieved successfully
    } else {
        return 1; // no profiles were found
    }
}

int get_profiles_by_habilities(Profile *profiles, char *habilities) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    // Open the database
    rc = sqlite3_open("profiles.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Prepare the SQL statement
    char *sql = sqlite3_mprintf("SELECT email, name, last_name FROM profiles WHERE habilities LIKE '%%%q%%'", habilities);
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        sqlite3_free(sql);
        return 1;
    }

    // Execute the statement
    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        strcpy(profiles[i].email, sqlite3_column_text(stmt, 0));
        strcpy(profiles[i].name, sqlite3_column_text(stmt, 1));
        strcpy(profiles[i].last_name, sqlite3_column_text(stmt, 2));
        i++;
    }

    // Finalize the statement and free memory
    sqlite3_finalize(stmt);
    sqlite3_free(sql);

    // Close the database
    sqlite3_close(db);

    if (i > 0) {
        return 0; // profiles were retrieved successfully
    } else {
        return 1; // no profiles were found
    }
}

int get_profiles_by_year_of_degree(Profile *profiles, int year_of_degree) {
    sqlite3 *db;
    char *err_msg = NULL;
    int rows = 0, cols = 0, count = 0;
    char **result;
    char sql[200];

    int rc = sqlite3_open("profiles.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sprintf(sql, "SELECT email, name, last_name, academic_degree FROM profiles WHERE year_of_degree = %d", year_of_degree);

    rc = sqlite3_get_table(db, sql, &result, &rows, &cols, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    for (int i = 0; i < rows; i++) {
        Profile profile;
        strcpy(profile.email, result[(i + 1) * cols + 0]);
        strcpy(profile.name, result[(i + 1) * cols + 1]);
        strcpy(profile.last_name, result[(i + 1) * cols + 2]);
        strcpy(profile.academic_degree, result[(i + 1) * cols + 3]);
        profiles[count++] = profile;
    }

    sqlite3_free_table(result);
    sqlite3_close(db);
    return 0;
}

int get_profiles(Profile *profiles) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_open("profiles.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_prepare_v2(db, "SELECT * FROM profiles", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        strcpy(profiles[i].email, (const char *)sqlite3_column_text(stmt, 0));
        strcpy(profiles[i].name, (const char *)sqlite3_column_text(stmt, 1));
        strcpy(profiles[i].last_name, (const char *)sqlite3_column_text(stmt, 2));
        strcpy(profiles[i].city, (const char *)sqlite3_column_text(stmt, 3));
        strcpy(profiles[i].academic_degree, (const char *)sqlite3_column_text(stmt, 4));
        profiles[i].year_of_degree = sqlite3_column_int(stmt, 5);
        strcpy(profiles[i].habilities, (const char *)sqlite3_column_text(stmt, 6));
        strcpy(profiles[i].image, (const char *)sqlite3_column_text(stmt, 7));
        i++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int get_profile_by_email(Profile *profile, char *email) {
    sqlite3 *db;
    char *err_msg = NULL;
    int rows = 0, cols = 0;
    char **result;
    char sql[200];

    int rc = sqlite3_open("profiles.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    sprintf(sql, "SELECT * FROM profiles WHERE email = '%s'", email);

    rc = sqlite3_get_table(db, sql, &result, &rows, &cols, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    if (rows == 0) {
        sqlite3_free_table(result);
        sqlite3_close(db);
        return 1; // No profile found
    }

    strcpy(profile->email, result[cols]);
    strcpy(profile->name, result[cols+1]);
    strcpy(profile->last_name, result[cols+2]);
    strcpy(profile->city, result[cols+3]);
    strcpy(profile->academic_degree, result[cols+4]);
    profile->year_of_degree = atoi(result[cols+5]);
    strcpy(profile->habilities, result[cols+6]);
    strcpy(profile->image, result[cols+7]);

    sqlite3_free_table(result);
    sqlite3_close(db);
    return 0;
}

int delete_profile_by_email(char *email) {
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("profiles.db", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *sql = sqlite3_mprintf("DELETE FROM profiles WHERE email='%q'", email);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to delete profile: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
