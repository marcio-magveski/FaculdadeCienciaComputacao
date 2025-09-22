#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define o tamanho m�ximo da nossa fila de pe�as
#define TAMANHO_FILA 5

// --- Estruturas de Dados ---

/**
 * @brief Define a estrutura de uma pe�a do jogo.
 * Cada pe�a tem um nome (seu formato) e um ID �nico.
 */
typedef struct {
    char nome;
    int id;
} Peca;

/**
 * @brief Define a estrutura da fila circular.
 * Guarda as pe�as, os �ndices de controle e a quantidade de itens.
 */
typedef struct {
    Peca itens[TAMANHO_FILA];
    int inicio;     // �ndice do primeiro elemento da fila.
    int fim;        // �ndice da pr�xima posi��o livre.
    int quantidade; // N�mero de pe�as atualmente na fila.
} Fila;


// --- Fun��es Auxiliares e de Gera��o ---

/**
 * @brief Gera uma nova pe�a de Tetris de forma aleat�ria.
 * As pe�as s�o criadas com um ID sequencial para rastreamento.
 * @return A nova pe�a criada.
 */
Peca gerarPeca() {
    // Contador est�tico para garantir que cada pe�a tenha um ID �nico.
    static int id_contador = 0;
    Peca nova_peca;
    char tipos_de_peca[] = {'I', 'O', 'T', 'L'};

    // Escolhe um tipo de pe�a aleatoriamente
    nova_peca.nome = tipos_de_peca[rand() % 4];
    nova_peca.id = id_contador++;

    return nova_peca;
}


// --- Fun��es de Manipula��o da Fila ---

/**
 * @brief Inicializa a fila, definindo seus valores padr�o.
 * Prepara a fila para ser usada.
 * @param f Ponteiro para a fila a ser inicializada.
 */
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

/**
 * @brief Adiciona uma nova pe�a ao final da fila (enqueue).
 * @param f Ponteiro para a fila.
 * @param p A pe�a a ser inserida.
 * @return 1 se a opera��o foi bem-sucedida, 0 se a fila estava cheia.
 */
int enfileirar(Fila *f, Peca p) {
    // Verifica se a fila n�o est� cheia antes de inserir.
    if (f->quantidade >= TAMANHO_FILA) {
        printf("\nAVISO: A fila de pecas esta cheia! Nao e possivel inserir.\n");
        return 0; // Retorna 0 para indicar falha (fila cheia)
    }

    f->itens[f->fim] = p;
    // Usa o operador de m�dulo para garantir que o �ndice 'fim' circule
    // de volta ao in�cio do array quando atingir o final.
    f->fim = (f->fim + 1) % TAMANHO_FILA;
    f->quantidade++;

    return 1; // Retorna 1 para indicar sucesso
}

/**
 * @brief Remove a pe�a da frente da fila (dequeue).
 * @param f Ponteiro para a fila.
 * @param p_removida Ponteiro para guardar a pe�a que foi removida.
 * @return 1 se a opera��o foi bem-sucedida, 0 se a fila estava vazia.
 */
int desenfileirar(Fila *f, Peca *p_removida) {
    // Verifica se a fila n�o est� vazia antes de remover.
    if (f->quantidade == 0) {
        printf("\nAVISO: A fila de pecas esta vazia! Nao ha pecas para jogar.\n");
        return 0; // Retorna 0 para indicar falha (fila vazia)
    }

    *p_removida = f->itens[f->inicio];
    // Move o in�cio para a pr�xima posi��o, circulando se necess�rio.
    f->inicio = (f->inicio + 1) % TAMANHO_FILA;
    f->quantidade--;

    return 1; // Retorna 1 para indicar sucesso
}


// --- Fun��es de Interface com o Usu�rio ---

/**
 * @brief Exibe o estado atual da fila de pe�as na tela.
 * A exibi��o percorre a fila de forma circular, do in�cio at� o fim.
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
            // Calcula o �ndice real no array circular
            indice_atual = (f->inicio + i) % TAMANHO_FILA;
            printf("[%c %d] ", f->itens[indice_atual].nome, f->itens[indice_atual].id);
        }
    }
    printf("\n");
}

/**
 * @brief Exibe o menu de op��es para o jogador.
 */
void exibirMenu() {
    printf("\nOpcoes de acao:\n");
    printf("1 - Jogar peca (dequeue)\n");
    printf("2 - Inserir nova peca (enqueue)\n");
    printf("0 - Sair\n");
    printf("Escolha sua acao: ");
}


// --- Fun��o Principal ---

int main() {
    // Prepara o gerador de n�meros aleat�rios
    srand(time(NULL));

    Fila fila_de_pecas;
    int opcao;

    // Inicia a fila e j� a preenche com o n�mero m�ximo de pe�as.
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
            case 1: { // Jogar pe�a (dequeue)
                Peca peca_jogada;
                if (desenfileirar(&fila_de_pecas, &peca_jogada)) {
                    printf("\n=> Peca jogada: [%c %d]\n", peca_jogada.nome, peca_jogada.id);
                }
                break;
            }
            case 2: { // Inserir nova pe�a (enqueue)
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
