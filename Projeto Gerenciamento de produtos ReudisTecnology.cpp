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

// Fun��o cadastrar um usu�rio
void cadastrarusuario(struct User *usuarios, int *qtd) {
    printf("Informe o nome da empresa: \n");
    scanf(" %[^\n]s", usuarios[*qtd].name);

    do {
        printf("Informe o email da empresa: \n");
        scanf(" %[^\n]s", usuarios[*qtd].email);
        
        if (!validarEmail(usuarios[*qtd].email)) {
            printf("Email inv�lido! Certifique-se de que cont�m '@' e um dom�nio v�lido.\n");
        }
    } while (!validarEmail(usuarios[*qtd].email));

    printf("Informe a senha do usu�rio: \n");
    scanf("%9s", usuarios[*qtd].pass);
    (*qtd)++;
    printf("Usu�rio cadastrado com sucesso!\n");
}

// Fun��o cadastrar produto
void cadastrarproduto() {
    struct Produto p;
    FILE *file = fopen("produtos.b", "ab");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("Informe o c�digo do produto: ");
    scanf("%d", &p.codigo);
    printf("Informe o nome do produto: ");
    scanf(" %[^\n]s", p.nome);
    printf("Informe o pre�o do produto: R$ ");
    scanf("%f", &p.preco);
    printf("Informe a quantidade em estoque: ");
    scanf("%d", &p.quantidade);

    fwrite(&p, sizeof(struct Produto), 1, file);
    fclose(file);
    printf("Produto cadastrado com sucesso!\n");
}

// Fun��o listar produtos
void listar() {
    struct Produto p;
    FILE *file = fopen("produtos.b", "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    printf("\n== Produtos Cadastrados ==\n");
    while (fread(&p, sizeof(struct Produto), 1, file)) {
        printf("C�digo: %d\n", p.codigo);
        printf("Nome: %s\n", p.nome);
        printf("Pre�o: R$ %.2f\n", p.preco);
        printf("Quantidade em estoque: %d\n\n", p.quantidade);
    }
    fclose(file);
}

// Fun��o editar produto
void editarProduto() {
    struct Produto p;
    FILE *file = fopen("produtos.b", "rb");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int codigo;
    printf("Informe o c�digo do produto que deseja editar: ");
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
            printf("Novo pre�o: R$ ");
            scanf("%f", &produtos[i].preco);
            printf("Nova quantidade: ");
            scanf("%d", &produtos[i].quantidade);
        }
        i++;
    }
    fclose(file);

    if (!encontrado) {
        printf("Produto com c�digo %d n�o encontrado.\n", codigo);
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

//fun��o excluir produto
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

    printf("Informe o c�digo do produto que deseja excluir:\n");
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

    printf("Produto exclu�do com sucesso, se ele existia.\n");
}

//fun��o sair do programa
void sair() {
    printf("Obrigado por usar o nosso sistema, a Reudis Tecnology agradece!. At� logo!\n");
}

// Fun��o login
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
    printf("Antes de prosseguir, por favor, fa�a o seu cadastro.\n");

    cadastrarusuario(usuarios, &qtdUsuarios);
    
    while (!logado) {
        logado = login(usuarios, qtdUsuarios);
    }

    do {
        menu();
        printf("Escolha uma op��o: ");
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
                printf("Op��o inv�lida!\n");
        }
    } while(op != 0);

    return 0;
}

