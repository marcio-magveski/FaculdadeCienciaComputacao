#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define a quantidade máxima de componentes que podem ser cadastrados.
#define MAX_COMPONENTES 20

// --- 1. Criação de Structs ---

/**
 * @struct Componente
 * @brief Estrutura para armazenar os dados de um componente da torre.
 * Contém nome, tipo e um nível de prioridade para a montagem.
 */
typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;


// --- Protótipos das Funções ---

void limparBufferEntrada();
void removerQuebraDeLinha(char *str);
void cadastrarComponente(Componente componentes[], int *total);
void mostrarComponentes(const Componente componentes[], int total);

// Algoritmos de Ordenação
void bubbleSortPorNome(Componente componentes[], int total, long long *comparacoes);
void insertionSortPorTipo(Componente componentes[], int total, long long *comparacoes);
void selectionSortPorPrioridade(Componente componentes[], int total, long long *comparacoes);

// Algoritmos de Busca
int buscaSequencialPorNome(const Componente componentes[], int total, const char *nomeBuscado, int *comparacoes);
int buscaBinariaPorNome(const Componente componentes[], int total, const char *nomeBuscado, int *comparacoes);


// --- Função Principal ---

int main() {
    Componente torre[MAX_COMPONENTES];
    int totalComponentes = 0;
    int ordenadoPorNome = 0; // Flag para controlar se a busca binária pode ser usada
    int opcao;

    do {
        printf("\n--- SISTEMA DE MONTAGEM DA TORRE DE RADIO ---\n");
        printf("1. Cadastrar Componente\n");
        printf("2. Listar Componentes Atuais\n");
        printf("3. Ordenar por Nome (Bubble Sort)\n");
        printf("4. Ordenar por Tipo (Insertion Sort)\n");
        printf("5. Ordenar por Prioridade (Selection Sort)\n");
        printf("6. Buscar Componente por Nome\n");
        printf("0. Sair\n");
        printf("---------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        // Variáveis para medir desempenho
        clock_t inicio, fim;
        double tempo_execucao;
        long long comparacoes = 0;

        switch (opcao) {
            case 1:
                cadastrarComponente(torre, &totalComponentes);
                ordenadoPorNome = 0; // Qualquer inserção "desordena" o vetor
                break;

            case 2:
                mostrarComponentes(torre, totalComponentes);
                break;

            case 3:
                printf("\nOrdenando por NOME com Bubble Sort...\n");
                inicio = clock();
                bubbleSortPorNome(torre, totalComponentes, &comparacoes);
                fim = clock();
                tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000; // em milissegundos
                printf("Ordenacao concluida!\n");
                printf("=> Comparacoes realizadas: %lld\n", comparacoes);
                printf("=> Tempo de execucao: %.4f ms\n", tempo_execucao);
                mostrarComponentes(torre, totalComponentes);
                ordenadoPorNome = 1; // Agora está ordenado por nome
                break;

            case 4:
                printf("\nOrdenando por TIPO com Insertion Sort...\n");
                inicio = clock();
                insertionSortPorTipo(torre, totalComponentes, &comparacoes);
                fim = clock();
                tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
                printf("Ordenacao concluida!\n");
                printf("=> Comparacoes realizadas: %lld\n", comparacoes);
                printf("=> Tempo de execucao: %.4f ms\n", tempo_execucao);
                mostrarComponentes(torre, totalComponentes);
                ordenadoPorNome = 0;
                break;

            case 5:
                printf("\nOrdenando por PRIORIDADE com Selection Sort...\n");
                inicio = clock();
                selectionSortPorPrioridade(torre, totalComponentes, &comparacoes);
                fim = clock();
                tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000;
                printf("Ordenacao concluida!\n");
                printf("=> Comparacoes realizadas: %lld\n", comparacoes);
                printf("=> Tempo de execucao: %.4f ms\n", tempo_execucao);
                mostrarComponentes(torre, totalComponentes);
                ordenadoPorNome = 0;
                break;

            case 6: {
                char nomeBusca[30];
                int comparacoesBusca = 0;
                int indice;

                printf("Digite o nome do componente a ser buscado: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                removerQuebraDeLinha(nomeBusca);

                // --- Busca Sequencial ---
                printf("\n--- Realizando Busca Sequencial ---\n");
                indice = buscaSequencialPorNome(torre, totalComponentes, nomeBusca, &comparacoesBusca);
                if (indice != -1) {
                    printf("Componente encontrado na posicao %d!\n", indice);
                } else {
                    printf("Componente nao encontrado.\n");
                }
                printf("=> Comparacoes realizadas: %d\n", comparacoesBusca);

                // --- Busca Binária (se aplicável) ---
                if (ordenadoPorNome) {
                    printf("\n--- Realizando Busca Binaria (vetor ordenado) ---\n");
                    comparacoesBusca = 0;
                    indice = buscaBinariaPorNome(torre, totalComponentes, nomeBusca, &comparacoesBusca);
                    if (indice != -1) {
                        printf("Componente encontrado na posicao %d!\n", indice);
                    } else {
                        printf("Componente nao encontrado.\n");
                    }
                    printf("=> Comparacoes realizadas: %d\n", comparacoesBusca);
                } else {
                    printf("\nAVISO: Busca Binaria nao pode ser executada. O vetor nao esta ordenado por nome.\n");
                }
                break;
            }
            
            case 0:
                printf("Encerrando sistema de montagem...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}


// --- Implementação das Funções Auxiliares ---

/**
 * @brief Limpa o buffer de entrada do teclado.
 * Essencial após usar scanf para ler números, antes de ler strings.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Remove o caractere de quebra de linha ('\n') do final de uma string lida com fgets.
 */
void removerQuebraDeLinha(char *str) {
    str[strcspn(str, "\n")] = 0;
}

/**
 * @brief Cadastra um novo componente no vetor.
 */
void cadastrarComponente(Componente componentes[], int *total) {
    if (*total >= MAX_COMPONENTES) {
        printf("Nao ha mais espaco para novos componentes!\n");
        return;
    }
    printf("\n--- Cadastro de Novo Componente ---\n");
    printf("Nome: ");
    fgets(componentes[*total].nome, sizeof(componentes[*total].nome), stdin);
    removerQuebraDeLinha(componentes[*total].nome);

    printf("Tipo (controle, suporte, propulsao): ");
    fgets(componentes[*total].tipo, sizeof(componentes[*total].tipo), stdin);
    removerQuebraDeLinha(componentes[*total].tipo);

    printf("Prioridade (1 a 10): ");
    scanf("%d", &componentes[*total].prioridade);
    limparBufferEntrada();
    
    (*total)++;
    printf("Componente cadastrado com sucesso!\n");
}

/**
 * @brief Exibe todos os componentes cadastrados de forma formatada.
 */
void mostrarComponentes(const Componente componentes[], int total) {
    if (total == 0) {
        printf("\nNenhum componente cadastrado ainda.\n");
        return;
    }
    printf("\n------------------- LISTA DE COMPONENTES -------------------\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < total; i++) {
        printf("%-20s | %-15s | %d\n", componentes[i].nome, componentes[i].tipo, componentes[i].prioridade);
    }
    printf("----------------------------------------------------------\n");
}


// --- Implementação dos Algoritmos de Ordenação ---

/**
 * @brief Ordena os componentes por NOME usando o algoritmo Bubble Sort.
 */
void bubbleSortPorNome(Componente componentes[], int total, long long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            (*comparacoes)++;
            if (strcmp(componentes[j].nome, componentes[j + 1].nome) > 0) {
                Componente temp = componentes[j];
                componentes[j] = componentes[j + 1];
                componentes[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Ordena os componentes por TIPO usando o algoritmo Insertion Sort.
 */
void insertionSortPorTipo(Componente componentes[], int total, long long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < total; i++) {
        Componente chave = componentes[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(chave.tipo, componentes[j].tipo) < 0) {
                componentes[j + 1] = componentes[j];
                j--;
            } else {
                break;
            }
        }
        componentes[j + 1] = chave;
    }
}

/**
 * @brief Ordena os componentes por PRIORIDADE usando o algoritmo Selection Sort.
 */
void selectionSortPorPrioridade(Componente componentes[], int total, long long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < total - 1; i++) {
        int indice_menor = i;
        for (int j = i + 1; j < total; j++) {
            (*comparacoes)++;
            if (componentes[j].prioridade < componentes[indice_menor].prioridade) {
                indice_menor = j;
            }
        }
        if (indice_menor != i) {
            Componente temp = componentes[i];
            componentes[i] = componentes[indice_menor];
            componentes[indice_menor] = temp;
        }
    }
}


// --- Implementação dos Algoritmos de Busca ---

/**
 * @brief Busca um componente pelo nome de forma sequencial.
 */
int buscaSequencialPorNome(const Componente componentes[], int total, const char *nomeBuscado, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < total; i++) {
        (*comparacoes)++;
        if (strcmp(componentes[i].nome, nomeBuscado) == 0) {
            return i; // Retorna o índice onde encontrou
        }
    }
    return -1; // Retorna -1 se não encontrou
}

/**
 * @brief Busca um componente pelo nome em um vetor ORDENADO.
 */
int buscaBinariaPorNome(const Componente componentes[], int total, const char *nomeBuscado, int *comparacoes) {
    *comparacoes = 0;
    int esquerda = 0, direita = total - 1;
    while (esquerda <= direita) {
        (*comparacoes)++;
        int meio = esquerda + (direita - esquerda) / 2;
        int res = strcmp(nomeBuscado, componentes[meio].nome);

        if (res == 0) return meio; // Encontrou
        if (res > 0) esquerda = meio + 1; // Busca na metade direita
        else direita = meio - 1; // Busca na metade esquerda
    }
    return -1; // Não encontrou
}
