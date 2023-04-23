#include <stdio.h>

#include "peekaboo_tui.h"
#include "../serializer/serializer.h"
#include "../stub/stub.h"

#include "../../utils/operation_codes.h"

void peekaboo_tui(int argc, char **argv)
{
    printf("Bem vindo ao Peekaboo!\n");
    printf("O Peekaboo é um banco de dados de perfis de pessoas.\n");
    printf("Para começar, você deseja:\n");
    printf("1 - Consultar perfis no Peekaboo?\n");
    printf("2 - Administrar os perfis no Peekaboo?\n");
    printf("3 - Sair do Peekaboo?\n");
    printf("Digite a opção desejada: ");

    int option;
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        printf("Conectando ao Peekaboo...\n");
        // TODO: User Mode
        user_mode_tui();
        break;
    case 2:
        printf("Para entrar no modo administrador, digite a senha: ");
        char password[100];
        scanf("%s", password);
        printf("Verificando senha...\n");
        // TODO: Validate password
        // TODO: Admin Mode
        break;
    case 3:
        printf("Saindo do Peekaboo...\n");
        break;
    default:
        printf("Opção inválida!\n");
        printf("Saindo do Peekaboo...\n");
        break;
    }

    return;
}

void user_mode_tui()
{
    OperationCode operation_code;
    char *serialized_operation;

    printf("Você está no modo usuário!\n");

    do
    {
        printf("O que você deseja fazer?\n");
        printf("1 - Listar todos os perfis formados em um determinado curso?\n");
        printf("2 - Listar todos os perfis que possuam uma determinada habilidade?\n");
        printf("3 - Listar todos os perfis formados em um determinado ano?\n");
        printf("4 - Listar todas as informações de todos os perfis?\n");
        printf("5 - Obter o perfil de uma pessoa a partir do seu email?\n");
        printf("0 - Encerrar o programa?\n");

        printf("Digite a opção desejada: ");
        scanf(" %d", (int *)&operation_code);

        switch (operation_code)
        {
        case LIST_BY_COURSE:
            printf("Digite o curso: ");
            char course[100];
            scanf(" %s", course);
            printf("Listando todos os perfis formados em %s...\n", course);
            // TODO: List by course
            serialized_operation = serialize_lbc_operation(course);
            break;
        case LIST_BY_SKILL:
            printf("Digite a habilidade: ");
            char skill[100];
            scanf(" %s", skill);
            printf("Listando todos os perfis que possuam a habilidade %s...\n", skill);
            // TODO: List by skill
            serialized_operation = serialize_lbs_operation(skill);
            break;
        case LIST_BY_YEAR:
            printf("Digite o ano: ");
            int year;
            scanf(" %d", &year);
            printf("Listando todos os perfis formados em %d...\n", year);
            // TODO: List by year
            serialized_operation = serialize_lby_operation(year);
            break;
        case LIST_ALL_PROFILES:
            printf("Listando todas as informações de todos os perfis...\n");
            // TODO: List all profiles
            serialized_operation = serialize_la_operation();
            break;
        case GET_PROFILE_BY_EMAIL:
            printf("Digite o email: ");
            char email[100];
            scanf(" %s", email);
            // TODO: Validate email
            printf("Obtendo o perfil de %s...\n", email);
            // TODO: Get profile by email
            serialized_operation = serialize_gp_operation(email);
            break;
        case EXIT:
            printf("Saindo do Peekaboo...\n");
            break;
        default:
            printf("Opção inválida!\n");
            printf("Escolha uma opção válida!\n");
            break;
        }

        printf("Serialized operation: %s\n", serialized_operation);

    } while (operation_code != 0);

    free(serialized_operation);
}

void admin_mode_tui()
{
    printf("Você está no modo administrador!\n");
}