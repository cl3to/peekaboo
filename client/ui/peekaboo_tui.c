#include <stdio.h>
#include <stdio.h>
#include <unistd.h>

#include "peekaboo_tui.h"
#include "../serializer/serializer.h"
#include "../stub/stub.h"
#include "../../utils/email_validator.h"

void welcome_messages(void)
{
    printf("Bem vindo ao Peekaboo!\n");
    printf("O Peekaboo é um banco de dados de perfis de pessoas.\n");
}

void peekaboo_tui(int argc, char **argv)
{
    int option;
    char *session_token = NULL;

    OperationCode operation_code;
    char *request = NULL, *response = NULL;
    int data_len = 0;
    char input[512];

    welcome_messages();

    do {
        printf("O que você deseja fazer?\n");
        printf("1 - Listar todos os perfis formados em um determinado curso?\n");
        printf("2 - Listar todos os perfis que possuam uma determinada habilidade?\n");
        printf("3 - Listar todos os perfis formados em um determinado ano?\n");
        printf("4 - Listar todas as informações de todos os perfis?\n");
        printf("5 - Obter o perfil de uma pessoa a partir do seu email?\n");
        if (!session_token)
            printf("6 - Logar como administrador?\n");
        else {
            printf("6 - Cadastrar um novo perfil?\n");
            printf("7 - Remover um perfil a partir do seu email?\n");
            printf("8 - Sair da conta de administrador?\n");
        }
        printf("0 - Encerrar o programa?\n");
        printf("Digite a opção desejada: ");

        scanf("%d", &option);

        if (option == 1)
        {
            printf("Digite o curso: ");
            scanf(" %s", input);
            printf("Listando todos os perfis formados em %s...\n", input);
            request = serialize_lbc_operation(input);  
            operation_code = LIST_BY_COURSE;
        }
        else if (option == 2)
        {
            printf("Digite a habilidade: ");
            scanf(" %s", input);
            printf("Listando todos os perfis que possuam a habilidade %s...\n", input);
            request = serialize_lbs_operation(input);
            operation_code = LIST_BY_SKILL;
        }
        else if (option == 3)
        {
            printf("Digite o ano: ");
            int year;
            scanf(" %d", &year);
            printf("Listando todos os perfis formados em %d...\n", year);
            request = serialize_lby_operation(year);
            operation_code = LIST_BY_YEAR;
        }
        else if (option == 4)
        {
            printf("Listando todas as informações de todos os perfis...\n");
            request = serialize_la_operation();
            operation_code = LIST_ALL_PROFILES;
        }
        else if (option == 5)
        {
            printf("Digite o email: ");
            scanf(" %s", input);
            while(check_email_format(input) != 0)
            {
                printf("Email Invalido!\n");
                printf("Digite novamente o email: ");
                scanf(" %s", input);
            }
            printf("Obtendo o perfil de %s...\n", input);
            request = serialize_gp_operation(input);
            operation_code = GET_PROFILE_BY_EMAIL;
        }
        else if (option == 6 && !session_token)
        {
            char *passwd = NULL;
            passwd = getpass("Digite a senha: ");
            request = serialize_login_operation(passwd);
            operation_code = LOGIN;
            printf("Logando como administrador...\n");
            response = make_request(request);
            session_token = deserialize_authentication(response);
            if (session_token)
                printf("Login realizado com sucesso!\n");
            else
                printf("Senha incorreta!\n");

            free(request);
            free(response);
            request = NULL;
            response = NULL;
            operation_code = LOGIN;
        }
        else if (session_token)
        {
            if (option == 6)
            {
                Profile new_profile;
                printf("Digite o email: ");
                scanf(" %s", new_profile.email);
                while(check_email_format(new_profile.email) != 0)
                {
                    printf("Email Invalido!\n");
                    printf("Digite novamente o email: ");
                    scanf(" %s", new_profile.email);
                }
                printf("Digite o nome: ");
                scanf(" %s", new_profile.name);
                printf("Digite o sobrenome: ");
                scanf(" %s", new_profile.last_name);
                printf("Digite a Formação Academica: ");
                scanf(" %s", new_profile.course);
                printf("Digite o ano de formação: ");
                scanf(" %d", &new_profile.year_of_degree);
                printf("Digite as habilidades (h1, h2, ..., hn): ");
                scanf(" %[^\n]", new_profile.skills);

                printf("Cadastrando o perfil de %s...\n", new_profile.email);
                request = serialize_cp_operation(&new_profile, session_token);
                operation_code = NEW_PROFILE;

            }
            else if (option == 7)
            {
                printf("Digite o email: ");
                scanf(" %s", input);
                while(check_email_format(input) != 0)
                {
                    printf("Email Invalido!\n");
                    printf("Digite novamente o email: ");
                    scanf(" %s", input);
                }
                printf("Removendo o perfil de %s...\n", input);
                request = serialize_rp_operation(input, session_token);
            }
            else if (option == 8)
            {
                printf("Saindo da conta de administrador...\n");
                request = serialize_logout_operation(session_token);
                operation_code = LOGOUT;
            }
        }

        else if (option == 0)
        {
            printf("Encerrando o programa...\n");
        }
        else
        {
            printf("Opção inválida!\n");
            printf("Escolha uma opção válida!\n");
        }

        if (request)
        {
            response = make_request(request);
            
            if (operation_code <= GET_PROFILE_BY_EMAIL)
            {
                Profile *profiles = deserialize_profile(response, &data_len);
                print_profile(profiles, data_len, operation_code);
                data_len = 0;
                free(profiles);

            }
            else if (session_token)
            {
                char *status_message = deserialize_admin_operation_response(response);
                if (status_message)
                    printf("%s\n", status_message);
                free(status_message);
            }

            free(request);
            free(response);
            request = NULL;
            response = NULL;
        }
    } while (option != 0);
}

void print_profile(Profile *profile, int data_len, OperationCode operation_code)
{
    if (data_len == 0)
    {
        printf("Nenhum perfil encontrado!\n");
        return;
    }

    for (int i = 0; i < data_len; i++)
    {
        printf("-------------PROFILE (%d)----------------\n", i + 1);
        printf("Email: %s\n", profile[i].email);
        printf("Nome: %s\n", profile[i].name);
        printf("Sobrenome: %s\n", profile[i].last_name);
        if (
            operation_code == LIST_BY_YEAR ||
            operation_code == LIST_ALL_PROFILES ||
            operation_code == GET_PROFILE_BY_EMAIL)
            printf("Formação Acadêmica: %s\n", profile[i].course);

        if (
            operation_code == LIST_ALL_PROFILES ||
            operation_code == GET_PROFILE_BY_EMAIL)
        {
            printf("Ano de Formação: %d\n", profile[i].year_of_degree);
            printf("Cidade: %s\n", profile[i].city);
            printf("Habilidades: %s\n", profile[i].skills);
        }
    }

    printf("-----------------------------------------\n");
}