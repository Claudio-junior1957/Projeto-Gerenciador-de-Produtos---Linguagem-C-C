#include <stdio.h>
#include <locale.h>
#include <string.h>


struct Produto {
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
};

struct User {
    char name[30];
    char email[50];
    char pass[10];
};


void menu() {
    printf("\n== Sistema de Cadastro de Produtos ==\n");
    printf("1 - Adicionar produto\n");
    printf("2 - Listar Produtos\n");
    printf("3 - Editar Produto\n");
    printf("4 - Excluir Produto\n");
    printf("0 - Sair\n");
    printf("====================================\n");
}

//funcao email
int validarEmail(char *email) {
    int atPosition = -1, dotPosition = -1;
    int length = strlen(email);

    for (int i = 0; i < length; i++) {
        if (email[i] == '@') {
            atPosition = i;
        } else if (email[i] == '.') {
            dotPosition = i;
        }
    }

    if (atPosition < 1 || dotPosition < atPosition + 2 || dotPosition >= length - 1) {
        return 0;
    }

    return 1;
}

// Função cadastrar um usuário
void cadastrarusuario(struct User *usuarios, int *qtd) {
    printf("Informe o nome da empresa: \n");
    scanf(" %[^\n]s", usuarios[*qtd].name);

    do {
        printf("Informe o email da empresa: \n");
        scanf(" %[^\n]s", usuarios[*qtd].email);
        
        if (!validarEmail(usuarios[*qtd].email)) {
            printf("Email inválido! Certifique-se de que contém '@' e um domínio válido.\n");
        }
    } while (!validarEmail(usuarios[*qtd].email));

    printf("Informe a senha do usuário: \n");
    scanf("%9s", usuarios[*qtd].pass);
    (*qtd)++;
    printf("Usuário cadastrado com sucesso!\n");
}

// Função cadastrar produto
void cadastrarproduto() {
    struct Produto p;
    FILE *file = fopen("produtos.b", "ab");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Informe o código do produto: ");
    scanf("%d", &p.codigo);
    printf("Informe o nome do produto: ");
    scanf(" %[^\n]s", p.nome);
    printf("Informe o preço do produto: R$ ");
    scanf("%f", &p.preco);
    printf("Informe a quantidade em estoque: ");
    scanf("%d", &p.quantidade);

    fwrite(&p, sizeof(struct Produto), 1, file);
    fclose(file);
    printf("Produto cadastrado com sucesso!\n");
}

// Função listar produtos
void listar() {
    struct Produto p;
    FILE *file = fopen("produtos.b", "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("\n== Produtos Cadastrados ==\n");
    while (fread(&p, sizeof(struct Produto), 1, file)) {
        printf("Código: %d\n", p.codigo);
        printf("Nome: %s\n", p.nome);
        printf("Preço: R$ %.2f\n", p.preco);
        printf("Quantidade em estoque: %d\n\n", p.quantidade);
    }
    fclose(file);
}

// Função editar produto
void editarProduto() {
    struct Produto p;
    FILE *file = fopen("produtos.b", "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int codigo;
    printf("Informe o código do produto que deseja editar: ");
    scanf("%d", &codigo);

    struct Produto produtos[100];
    int i = 0, encontrado = 0;
    while (fread(&p, sizeof(struct Produto), 1, file)) {
        produtos[i] = p;
        if (p.codigo == codigo) {
            encontrado = 1;
            printf("Produto encontrado! Informe os novos dados.\n");
            printf("Novo nome: ");
            scanf(" %[^\n]s", produtos[i].nome);
            printf("Novo preço: R$ ");
            scanf("%f", &produtos[i].preco);
            printf("Nova quantidade: ");
            scanf("%d", &produtos[i].quantidade);
        }
        i++;
    }
    fclose(file);

    if (!encontrado) {
        printf("Produto com código %d não encontrado.\n", codigo);
        return;
    }

    file = fopen("produtos.b", "wb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int j = 0; j < i; j++) {
        fwrite(&produtos[j], sizeof(struct Produto), 1, file);
    }
    fclose(file);
    printf("Produto editado com sucesso!\n");
}

//função excluir produto
void excluirProduto() {
    FILE *file = fopen("produtos.b", "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    struct Produto p;
    int tam = 0;
    while (fread(&p, sizeof(struct Produto), 1, file)) {
        tam++;
    }
    fclose(file);

    file = fopen("produtos.b", "rb");
    struct Produto produtos[tam];
    fread(produtos, sizeof(struct Produto), tam, file);
    fclose(file);

    printf("Informe o código do produto que deseja excluir:\n");
    int codigo;
    scanf("%d", &codigo);

    struct Produto temp[tam];
    int j = 0;
    for (int i = 0; i < tam; i++) {
        if (produtos[i].codigo != codigo) {
            temp[j] = produtos[i];
            j++;
        }
    }

    file = fopen("produtos.b", "wb");
    fwrite(temp, sizeof(struct Produto), j, file);
    fclose(file);

    printf("Produto excluído com sucesso, se ele existia.\n");
}

//função sair do programa
void sair() {
    printf("Obrigado por usar o nosso sistema, a Reudis Tecnology agradece!. Até logo!\n");
}

// Função login
int login(struct User *usuarios, int qtd) {
    char email[50];
    char senha[10];
    char nomeEmpresa[30];
    
    printf("\n=== Login ===\n");
    printf("Informe o nome da empresa: ");
    scanf(" %[^\n]s", nomeEmpresa);
    printf("Informe o email: ");
    scanf(" %[^\n]s", email);
    printf("Informe a senha: ");
    scanf("%9s", senha);
    
    for (int i = 0; i < qtd; i++) {
        if (strcmp(usuarios[i].name, nomeEmpresa) == 0 &&
            strcmp(usuarios[i].email, email) == 0 &&
            strcmp(usuarios[i].pass, senha) == 0) {
            printf("Cadastro realizado com sucesso!\n");
            return 1;
        }
    }
    
    printf("Login falhou! Credenciais incorretas.\n");
    return 0;
}

int main() {
    setlocale(LC_ALL, ""); 
    struct User usuarios[100];
    int qtdUsuarios = 0;
    int op;
    int logado = 0;

    printf("Bem-vindo ao Sistema de Cadastro de Produtos da Reudis Tecnology!\n");
    printf("Antes de prosseguir, por favor, faça o seu cadastro.\n");

    cadastrarusuario(usuarios, &qtdUsuarios);
    
    while (!logado) {
        logado = login(usuarios, qtdUsuarios);
    }

    do {
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &op);

        switch(op) {
            case 1:
                cadastrarproduto();
                break;
            case 2:
                listar();
                break;
            case 3:
                editarProduto();
                break;
            case 4:
                excluirProduto();
                break;
            case 0:
                sair();
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while(op != 0);

    return 0;
}

