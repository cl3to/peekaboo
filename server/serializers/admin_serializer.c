#include "admin_serializer.h"


#define BUFFER_SIZE 512
#define SESSION_TOKEN_LENGTH 42

char expected_session_token[SESSION_TOKEN_LENGTH+1];

int generate_session_token(){
    static char valid_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~+/#$@&*?!";        
    
    srand(time(NULL));
    for (int c = 0; c < SESSION_TOKEN_LENGTH; c++) {            
        int key = rand() % (int)(sizeof(valid_chars) - 1);
        expected_session_token[c] = valid_chars[key];
    }

    expected_session_token[SESSION_TOKEN_LENGTH] = '\0';
    return 1;
}

int validate_session_token(char *session_token){
  if(session_token!= NULL && strcmp(session_token, expected_session_token) == 0){
    return 0;
  }
  printf("Invalid session token.\n");
  return -1;
}

char* validate_password(char *password) {

    // Expected password hash 
    unsigned char expected_hash[] = {
      0x25, 0x0e, 0x77, 0xf1, 0x2a, 0x5a, 0xb6, 0x97,
      0x2a, 0x08, 0x95, 0xd2, 0x90, 0xc4, 0x79, 0x2f,
      0x0a, 0x32, 0x6e, 0xa8
    };

    unsigned char hash[SHA_DIGEST_LENGTH];

    SHA1(password, strlen(password), hash);

    // Compare generated hash with expected hash
    if (memcmp(hash, expected_hash, SHA_DIGEST_LENGTH) == 0) {
        generate_session_token();
        return strdup(expected_session_token);
    }
    printf("The password provided is invalid.\n");
    return NULL;
}

char* login_with_password(char *password){
  char* new_session_token = validate_password(password);
  
  if(new_session_token == NULL){
    return NULL;
  }

  // TODO: Melhorar a criação da mensagem depois de confirmar formato
  char login_response[BUFFER_SIZE];
  sprintf(
    login_response, 
    "{\"operation_code\": %d,\"params_length\": 1,\"params\":{\"session_token\": \"%s\"}}",
    LOGIN,
    new_session_token
  );

  return strdup(login_response);
}

int logout(char *session_token){
  if (validate_session_token(session_token)==0){
    memset(expected_session_token, '\0', SESSION_TOKEN_LENGTH);
    return 0;
  }
  return -1;
}

// TODO: create a response message
int create_new_profile(char *session_token, char *email, char *name, char *last_name, char *city, char *academic_degree, int year_of_degree, char *habilities){
  if (validate_session_token(session_token)==0){
    
    Profile *profile = new_profile(
        email, name, last_name, city, academic_degree, year_of_degree, habilities
    );

    if(profile == NULL){
      return -1;
    }

    if(store_profile(profile)<0){
      return -1;
    }
    
    return 0;
  }
  return -1;
}

// TODO: create a response message
int remove_profile_by_email(char *session_token, char *email){
  if (validate_session_token(session_token)==0){
    
    //TODO: Apply regex on the email
    
    if(delete_profile_by_email(email) == 0){
      return 0;
    }
  }
  return -1;
}
