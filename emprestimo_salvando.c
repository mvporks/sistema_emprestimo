#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Codigo permite inserir novos empréstimos, imprimir a lista na tela, salvar a lista em um arquivo txt e exportar arquivo txt

// Variável global para armazenar o nome do arquivo
char nomeArquivo[100];

typedef struct No
{
    char nome[30];
    float renda, valor_emprestado, valor_parcela;
    int parcela;
    struct No *proximo;
} No;

// Lista Simplesmente Encadeada, cada nó possui campos para armazenar as funções
// e um ponteiro para o próximo nó da lista.

No *cabeca = NULL; // Cabeça da lista

void inserirNo(const char *nome, float renda, float valor_emprestado, int parcela)
{
    No *novoNo = (No *)malloc(sizeof(No));
    if (novoNo == NULL)
    {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }

    strcpy(novoNo->nome, nome);
    novoNo->renda = renda;
    novoNo->valor_emprestado = valor_emprestado;
    novoNo->parcela = parcela;
    novoNo->valor_parcela = valor_emprestado / parcela;
    novoNo->proximo = NULL;

    // inserirNo é responsável por criar um novo nó e inseri-lo no final da lista recebendo os parametros
    // Primeiro, é alocada memória para o novo nó usando a função malloc.
    // Com o preenchimento dos dados. Caso a lista estiver NULL, o nó inserido se torna a cabeça da lista.
    // Com valores a lista sera percorrida até ultimo nó e atualizado, um novo nó

    if (cabeca == NULL)
    {
        cabeca = novoNo;
    }
    else
    {
        No *atual = cabeca;
        while (atual->proximo != NULL)
        {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
    }
}

void imprimirLista() // imprimir a lista do inicio ao fim, ate o ponteiro ser NULL
{
    No *atual = cabeca;
    while (atual != NULL)
    {
        printf("\nNome da pessoa: %s", atual->nome);
        printf("\nRenda da pessoa: R$%.2f", atual->renda);
        printf("\nValor emprestado para pessoa: R$%.2f", atual->valor_emprestado);
        printf("\nQuantidade de parcelas do empréstimo: %d", atual->parcela);
        printf("\nValor das parcelas do empréstimo: R$%.2f \n\n", atual->valor_parcela);
        atual = atual->proximo;
    }
}
// O salvar recebe o nome do arquivo como parâmetro e abre o arquivo em modo de escrita. Depois percorre os nós e escreve as info dos emprestimos
// Os campos do nó são escritos em sequência,separados por espaços, usando a função fprintf.
// O arquivo é fechado com fclose.
void salvarListaEmArquivo(const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    No *atual = cabeca;
    while (atual != NULL)
    {
        fprintf(arquivo, "%s %.2f %.2f %d %.2f\n", atual->nome, atual->renda,
                atual->valor_emprestado, atual->parcela, atual->valor_parcela);
        atual = atual->proximo;
    }

    fclose(arquivo);
    printf("Lista salva no arquivo com sucesso.\n");
}

void lerListaDeArquivo(const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char nome[30];
    float renda, valor_emprestado, valor_parcela;
    int parcela;

    while (fscanf(arquivo, "%s %f %f %d %f\n", nome, &renda, &valor_emprestado, &parcela, &valor_parcela) != EOF)
    {
        inserirNo(nome, renda, valor_emprestado, parcela);
    }

    fclose(arquivo);
    printf("Lista carregada do arquivo com sucesso.\n");
}

void novo_emprestimo()
{
    char nome[30];
    float renda, valor_emprestado;
    int parcela;

    printf("Digite o nome da pessoa: ");
    scanf("%s", nome);

    printf("Digite a renda da pessoa R$: ");
    scanf("%f", &renda);

    float max_emprestimo = renda * 0.2 * 24; // Calcular o valor máximo de empréstimo (20% da renda)

    printf("Valor maximo de emprestimo disponivel: R$%.2f\n", max_emprestimo);

    printf("Digite o valor desejado para emprestimo: R$");
    scanf("%f", &valor_emprestado);

    if (valor_emprestado > max_emprestimo)
    {
        printf("Valor de emprestimo invalido.\n");
        return;
    }

    float max_valor_parcela = renda * 0.2; // Calcular o valor máximo por parcela (20% da renda)

    int min_qtd_parcela = valor_emprestado / max_valor_parcela; // Calcular a quantidade mínima de parcelas para não afetar a renda mensal

    printf("Quantidade mínima de parcelas para não afetar a renda mensal: %d\n", min_qtd_parcela);

    printf("Digite a quantidade de parcelas desejada: ");
    scanf("%d", &parcela);

    if (parcela < min_qtd_parcela)
    {
        printf("Quantidade de parcelas inválida.\n");
        return;
    }

    float valor_parcela = valor_emprestado / parcela;

    printf("O valor do empréstimo de R$%.2f será pago em %d parcela(s) de R$%.2f\n", valor_emprestado, parcela, valor_parcela);

    printf("Deseja confirmar o empréstimo? (S/N): ");
    char resposta;
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's')
    {
        inserirNo(nome, renda, valor_emprestado, parcela);
        printf("Empréstimo confirmado.\n");

        salvarListaEmArquivo(nomeArquivo); // Salvar a lista atualizada no arquivo especificado anteriormente
    }
    else
    {
        printf("Empréstimo cancelado.\n");
    }
}

void menu_sistema()
{
    int opcao;

    printf("\nSelecione uma opcao:\n");
    printf("1. Visualizar todos os emprestimos\n");
    printf("2. Cadastrar um novo emprestimo\n");
    printf("3. Encerrar aplicacao\n");
    printf("Opcao: ");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        printf("\nLista de emprestimos:\n");
        imprimirLista();
        break;
    case 2:
        novo_emprestimo();
        break;
    case 3:
        printf("Encerrando aplicação.\n");
        exit(0);
    default:
        printf("Opção inválida.\n");
    }

    printf("\n");
    menu_sistema();
}

int main()
{
    int opcao;

    printf("Selecione uma opção:\n");
    printf("1. Gerar novo arquivo TXT e ler exemplos no código.\n");
    printf("2. Ler lista de empréstimos de um arquivo TXT.\n");
    printf("Opção: ");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        inserirNo("João", 2000.0, 1000.0, 5);
        inserirNo("Maria", 3000.0, 1500.0, 8);
        inserirNo("Pedro", 2500.0, 2000.0, 10);
        printf("Exemplos inseridos na lista.\n");

        // Salvar a lista atualizada no arquivo
        printf("Digite o nome do arquivo TXT: ");
        scanf("%s", nomeArquivo);
        salvarListaEmArquivo(nomeArquivo);
        break;
    case 2:
        printf("Digite o nome do arquivo TXT: ");
        scanf("%s", nomeArquivo);
        lerListaDeArquivo(nomeArquivo);
        break;
    default:
        printf("Opção inválida.\n");
        exit(1);
    }

    menu_sistema();

    return 0;
}
