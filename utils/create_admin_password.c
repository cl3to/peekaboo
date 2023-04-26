#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <unistd.h>

int main()
{
  char *password, *validate_password;

  password = strdup(getpass("Digite uma senha: "));
  validate_password = strdup(getpass("Confime a senha: "));

  while (strcmp(password, validate_password) != 0)
  {
    printf("As senhas fornecidas não correspondem!\n");

    password = strdup(getpass("Digite uma senha: "));
    validate_password = strdup(getpass("Confime a senha: "));
  }

  // Calculate SHA-1 hash of password
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1(password, strlen(password), hash);

  // Open file for writing
  FILE *fp;
  fp = fopen("../password_sha1.pwd", "w");

  fprintf(fp, "%s", hash);

  fclose(fp);

  printf("Senha salva com sucesso!\n");

  return 0;
}