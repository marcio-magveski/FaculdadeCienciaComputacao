#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define o tamanho máximo da nossa fila de peças
#define TAMANHO_FILA 5

// --- Estruturas de Dados ---

/**
 * @brief Define a estrutura de uma peça do jogo.
 * Cada peça tem um nome (seu formato) e um ID único.
 */
typedef struct {
    char nome;
    int id;
} Peca;

/**
 * @brief Define a estrutura da fila circular.
 * Guarda as peças, os índices de controle e a quantidade de itens.
 */
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;     // Índice do primeiro elemento da fila.
    int fim;        // Índice da próxima posição livre.
    int quantidade; // Número de peças atualmente na fila.
} Fila;


// --- Funções Auxiliares e de Geração ---

/**
 * @brief Gera uma nova peça de Tetris de forma aleatória.
 * As peças são criadas com um ID sequencial para rastreamento.
 * @return A nova peça criada.
 */
Peca gerarPeca() {
    // Contador estático para garantir que cada peça tenha um ID único.
    static int id_contador = 0;
    Peca nova_peca;
    char tipos_de_peca[] = {'I', 'O', 'T', 'L'};

    // Escolhe um tipo de peça aleatoriamente
    nova_peca.nome = tipos_de_peca[rand() % 4];
    nova_peca.id = id_contador++;

    return nova_peca;
}


// --- Funções de Manipulação da Fila ---

/**
 * @brief Inicializa a fila, definindo seus valores padrão.
 * Prepara a fila para ser usada.
 * @param f Ponteiro para a fila a ser inicializada.
 */
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

/**
 * @brief Adiciona uma nova peça ao final da fila (enqueue).
 * @param f Ponteiro para a fila.
 * @param p A peça a ser inserida.
 * @return 1 se a operação foi bem-sucedida, 0 se a fila estava cheia.
 */
int enfileirar(Fila *f, Peca p) {
    // Verifica se a fila não está cheia antes de inserir.
    if (f->quantidade >= TAMANHO_FILA) {
        printf("\nAVISO: A fila de pecas esta cheia! Nao e possivel inserir.\n");
        return 0; // Retorna 0 para indicar falha (fila cheia)
    }

    f->itens[f->fim] = p;
    // Usa o operador de módulo para garantir que o índice 'fim' circule
    // de volta ao início do array quando atingir o final.
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->quantidade++;

    return 1; // Retorna 1 para indicar sucesso
}

/**
 * @brief Remove a peça da frente da fila (dequeue).
 * @param f Ponteiro para a fila.
 * @param p_removida Ponteiro para guardar a peça que foi removida.
 * @return 1 se a operação foi bem-sucedida, 0 se a fila estava vazia.
 */
int desenfileirar(Fila *f, Peca *p_removida) {
    // Verifica se a fila não está vazia antes de remover.
    if (f->quantidade == 0) {
        printf("\nAVISO: A fila de pecas esta vazia! Nao ha pecas para jogar.\n");
        return 0; // Retorna 0 para indicar falha (fila vazia)
    }

    *p_removida = f->itens[f->inicio];
    // Move o início para a próxima posição, circulando se necessário.
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;

    return 1; // Retorna 1 para indicar sucesso
}


// --- Funções de Interface com o Usuário ---

/**
 * @brief Exibe o estado atual da fila de peças na tela.
 * A exibição percorre a fila de forma circular, do início até o fim.
 * @param f Ponteiro para a fila a ser exibida.
 */
void exibirFila(Fila *f) {
    printf("\nFila de pecas: ");
    if (f->quantidade == 0) {
        printf("[VAZIA]");
    } else {
        int i;
        int indice_atual;
        for (i = 0; i < f->quantidade; i++) {
            // Calcula o índice real no array circular
            indice_atual = (f->inicio + i) % TAMANHO_FILA;
            printf("[%c %d] ", f->itens[indice_atual].nome, f->itens[indice_atual].id);
        }
    }
    printf("\n");
}

/**
 * @brief Exibe o menu de opções para o jogador.
 */
void exibirMenu() {
    printf("\nOpcoes de acao:\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair\n");
    printf("Escolha sua acao: ");
}


// --- Função Principal ---

int main() {
    // Prepara o gerador de números aleatórios
    srand(time(NULL));

    Fila fila_de_pecas;
    int opcao;

    // Inicia a fila e já a preenche com o número máximo de peças.
    inicializarFila(&fila_de_pecas);
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enfileirar(&fila_de_pecas, gerarPeca());
    }

    // Loop principal do jogo
    do {
        exibirFila(&fila_de_pecas);
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça (dequeue)
                Peca peca_jogada;
                if (desenfileirar(&fila_de_pecas, &peca_jogada)) {
                    printf("\n=> Peca jogada: [%c %d]\n", peca_jogada.nome, peca_jogada.id);
                }
                break;
            }
            case 2: { // Inserir nova peça (enqueue)
                Peca nova_peca = gerarPeca();
                if (enfileirar(&fila_de_pecas, nova_peca)) {
                     printf("\n=> Nova peca inserida: [%c %d]\n", nova_peca.nome, nova_peca.id);
                }
                break;
            }
            case 0: { // Sair
                printf("\nFim de jogo. Ate a proxima!\n");
                break;
            }
            default: {
                printf("\nOpcao invalida. Tente novamente.\n");
            }
        }

    } while (opcao != 0);

    return 0;
}
