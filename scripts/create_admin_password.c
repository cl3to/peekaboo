#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../lib/sha1-c/sha1.h"

// Create a new password for the admin's system
int main()
{
  // Read a password from the terminal
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
  SHA1Context hash;
  SHA1Reset(&hash);
  SHA1Input(&hash, (const unsigned char *)password, strlen(password));

  if (!SHA1Result(&hash))
  {
    fprintf(stderr, "Ocorreu um erro durante a criação da senha. Realize a ação novamente.\n");
    return -1;
  }
  else
  {

    // Open file for writing
    FILE *fp;
    fp = fopen("../password_sha1.pwd", "wb");

    if (fp == NULL)
    {
      printf("Erro ao salva a senha. Realize a ação novamente.\n");
      return -1;
    }

    // Save the given password in the password_sha1.pwd file
    fprintf(fp, "%08x%08x%08x%08x%08x",
            hash.Message_Digest[0],
            hash.Message_Digest[1],
            hash.Message_Digest[2],
            hash.Message_Digest[3],
            hash.Message_Digest[4]);

    fclose(fp);

    printf("Senha salva com sucesso!\n");
  }

  return 0;
}