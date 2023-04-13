#include "model/profile.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{

    Profile *profile = new_profile(
        "j@fht.com", "Juan", "Perez", "Bogota", "Ingeniero", 2010, "C, C++, Java"
    );

    Profile *profile2 = new_profile(
        "e@fht.com", "Elias", "Novato", "Cancún", "Masón", 2014, "Construcción de viviendas, Muro de nivelación, Acabado"
    );

    store_profile(profile);
    store_profile(profile2);

    Profile *profiles  = (Profile *) malloc(sizeof(Profile) * 2);

    //get_profiles(profiles);

    // get_profiles_by_academic_degree(profiles, "Masón");

    // get_profiles_by_habilities(profiles, "A");

    // get_profiles_by_year_of_degree(profiles, 2014);

    // get_profile_by_email(profiles, "e@fht.com");

    // delete_profile_by_email("e@fht.com");

    get_profiles(profiles);

    for (int i = 0; i < 2; i++) {
        printf("Profile %d:\n", i);
        printf("Email: %s\n", profiles[i].email);
        printf("Name: %s\n", profiles[i].name);
        printf("Last name: %s\n", profiles[i].last_name);
        printf("City: %s\n", profiles[i].city);
        printf("Academic degree: %s\n", profiles[i].academic_degree);
        printf("Year of degree: %d\n", profiles[i].year_of_degree);
        printf("Habilities: %s\n", profiles[i].habilities);
        printf("Image: %s\n", profiles[i].image);
    }

    free(profile);
    free(profile2);
    free(profiles);

    return 0;

}
