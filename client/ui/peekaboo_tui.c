#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "peekaboo_tui.h"
#include "../../utils/constants.h"
#include "../serializer/serializer.h"
#include "../../utils/email_validator.h"

void clear_screen() {
    system("clear");
}

void welcome_messages(void)
{
    printf("Bem vindo ao Peekaboo!\n");
    printf("O Peekaboo é um banco de dados de perfis de pessoas.\n");
}

void print_profile(Profile *profile, int data_len, OperationCode operation_code)
{
    if (data_len == 0)
    {
        printf("-----------------------------------------\n");
        printf("Nenhum perfil encontrado!\n");
        printf("-----------------------------------------\n");
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

void email_input(char *email)
{
        printf("Digite o email: ");
        scanf(" %s", email);
        while(check_email_format(email) != 0)
        {
            printf("Email Invalido!\n");
            printf("Digite novamente o email: ");
            scanf(" %s", email);
        }
}

void peekaboo_tui(ConnectionHandler *conn_handler)
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
            scanf(" %[^\n]", input);
            printf("Listando todos os perfis formados em %s...\n", input);
            request = serialize_lbc_operation(input);  
            operation_code = LIST_BY_COURSE;
        }
        else if (option == 2)
        {
            printf("Digite a habilidade: ");
            scanf(" %[^\n]", input);
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
            email_input(input);
            printf("Obtendo o perfil de %s...\n", input);
            request = serialize_gp_operation(input);
            operation_code = GET_PROFILE_BY_EMAIL;
        }
        else if (option == 6 && !session_token)
        {
            char *passwd = NULL;
            passwd = getpass("Digite a senha: ");
            request = serialize_login_operation(passwd);
            printf("Logando como administrador...\n");
            operation_code = LOGIN;
        }
        else if (session_token)
        {
            if (option == 6)
            {
                Profile new_profile;
                email_input(new_profile.email);
                printf("Digite o nome: ");
                scanf(" %[^\n]", new_profile.name);
                printf("Digite o sobrenome: ");
                scanf(" %[^\n]", new_profile.last_name);
                printf("Digite a cidade de residência: ");
                scanf(" %[^\n]", new_profile.city);
                printf("Digite a Formação Academica: ");
                scanf(" %[^\n]", new_profile.course);
                printf("Digite o ano de formatura: ");
                scanf(" %d", &new_profile.year_of_degree);
                printf("Digite as habilidades (h1, h2, ..., hn): ");
                scanf(" %[^\n]", new_profile.skills);

                printf("Cadastrando o perfil de %s...\n", new_profile.email);
                request = serialize_cp_operation(&new_profile, session_token);
                operation_code = NEW_PROFILE;
            }
            else if (option == 7)
            {
                email_input(input);
                printf("Removendo o perfil de %s...\n", input);
                request = serialize_rp_operation(input, session_token);
                operation_code = REMOVE_PROFILE_BY_EMAIL;
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
            conn_handler->disconnect(conn_handler);
            operation_code = EXIT;
        }
        else
        {
            printf("Opção inválida!\n");
            printf("Escolha uma opção válida!\n");
        }

        clear_screen();

        if (request)
        {
            response = make_request(conn_handler, request);
            
            if (operation_code <= GET_PROFILE_BY_EMAIL)
            {
                Profile *profiles = deserialize_profile(response, &data_len);
                print_profile(profiles, data_len, operation_code);
                data_len = 0;
                free(profiles);

            }
            else if (operation_code == LOGIN)
            {
                printf("-----------------------------------------\n");
                session_token = deserialize_authentication(response);
                if (session_token)
                    printf("Login realizado com sucesso!\n");
                else
                    printf("Senha incorreta!\n");
                printf("-----------------------------------------\n");
                
            }
            else if (session_token)
            {
                int status;
                char *status_message = deserialize_admin_operation_response(response, &status);

                printf("-----------------------------------------\n");
                if (status == 200)
                    printf("Operação realizada com sucesso!\n");
                else
                    printf("%s\n", status_message);
                printf("-----------------------------------------\n");

                if (operation_code == LOGOUT && status == 200)
                    session_token = NULL;

                free(status_message);
            }

            free(request);
            free(response);
            request = NULL;
            response = NULL;
        }
    }
    while (option != 0);
}

void fail_connection()
{
    printf("Não foi possível estabelecer uma conexão com o servidor!\n");
    printf("Encerrando o programa...\n");
}