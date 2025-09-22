#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para tolower()

// --- 1. DEFINIÇÃO DAS ESTRUTURAS ---

#define HASH_SIZE 101

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct NoPista {
    char pista[100];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

typedef struct HashItem {
    char chave[100];
    char valor[50];
    struct HashItem *proximo;
} HashItem;


// --- 2. MÓDULO DA TABELA HASH ---

HashItem* tabelaHash[HASH_SIZE];

// ---- FUNÇÕES CORRIGIDAS ----
unsigned int funcaoHash(const char *chave) {
    unsigned long int hash = 0;
    int c;
    while ((c = *chave++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash % HASH_SIZE;
}

void inserirNaHash(const char *chave, const char *valor) {
    HashItem *novoItem = (HashItem*) malloc(sizeof(HashItem));
    strcpy(novoItem->chave, chave);
    strcpy(novoItem->valor, valor);
    novoItem->proximo = NULL;

    unsigned int indice = funcaoHash(chave);
    novoItem->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoItem;
}

char* encontrarSuspeito(const char *chave) {
    unsigned int indice = funcaoHash(chave);
    HashItem *item = tabelaHash[indice];
    while (item != NULL) {
        if (strcmp(item->chave, chave) == 0) {
            return item->valor;
        }
        item = item->proximo;
    }
    return NULL;
}


// --- 3. MÓDULO DA ÁRVORE DE PISTAS (BST) ---

// ---- FUNÇÃO CORRIGIDA ----
NoPista* inserirPista(NoPista* raiz, const char* pista) {
    if (raiz == NULL) {
        NoPista* novoNo = (NoPista*) malloc(sizeof(NoPista));
        strcpy(novoNo->pista, pista);
        novoNo->esquerda = novoNo->direita = NULL;
        return novoNo;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

void mostrarPistas(NoPista* raiz) {
    if (raiz != NULL) {
        mostrarPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        mostrarPistas(raiz->direita);
    }
}


// --- 4. MÓDULO DA MANSÃO (ÁRVORE BINÁRIA) ---

// ---- FUNÇÃO CORRIGIDA ----
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*) malloc(sizeof(Sala));
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

void explorarSalas(Sala* salaInicial, NoPista** raizPistas) {
    Sala* salaAtual = salaInicial;
    char escolha;
    printf("Voce esta no(a) %s. A mansao e vasta e escura...\n", salaAtual->nome);
    while (salaAtual != NULL) {
        if (strlen(salaAtual->pista) > 0) {
            printf("Voce encontrou algo! Pista: [ %s ]\n", salaAtual->pista);
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            strcpy(salaAtual->pista, "");
        }
        printf("\nVoce esta no(a): %s\n", salaAtual->nome);
        printf("Caminhos disponiveis:\n");
        if (salaAtual->esquerda) printf("  (e)squerda -> %s\n", salaAtual->esquerda->nome);
        if (salaAtual->direita) printf("  (d)ireita -> %s\n", salaAtual->direita->nome);
        printf("  (s)air da exploracao e ir para o julgamento\n");
        printf("Para onde voce vai? ");
        scanf(" %c", &escolha);
        escolha = tolower(escolha);
        if (escolha == 'e' && salaAtual->esquerda != NULL) {
            salaAtual = salaAtual->esquerda;
        } else if (escolha == 'd' && salaAtual->direita != NULL) {
            salaAtual = salaAtual->direita;
        } else if (escolha == 's') {
            printf("\nVoce sente que ja tem o suficiente. Hora de acusar o culpado.\n");
            break;
        } else {
            printf("Caminho invalido. Tente novamente.\n");
        }
    }
}

int contarPistasParaSuspeito(NoPista* noPista, const char* suspeitoAcusado) {
    if (noPista == NULL) return 0;
    int contagem = 0;
    char* suspeitoDaPista = encontrarSuspeito(noPista->pista);
    if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, suspeitoAcusado) == 0) {
        contagem = 1;
    }
    return contagem + contarPistasParaSuspeito(noPista->esquerda, suspeitoAcusado) + contarPistasParaSuspeito(noPista->direita, suspeitoAcusado);
}

void verificarSuspeitoFinal(NoPista* raizPistas) {
    char acusado[50];
    printf("\n--- O JULGAMENTO ---\n");
    if (raizPistas == NULL) {
        printf("Voce nao coletou nenhuma pista. O caso permanece um misterio.\n");
        return;
    }
    printf("Pistas que voce coletou:\n");
    mostrarPistas(raizPistas);
    printf("\nSuspeitos: Mordomo, Jardineiro, Governanta, Dama_de_Companhia\n");
    printf("Com base nas evidencias, quem voce acusa? ");
    scanf("%49s", acusado); // Limita a entrada para evitar overflow
    int numPistas = contarPistasParaSuspeito(raizPistas, acusado);
    printf("\nAnalisando as evidencias contra %s...\n", acusado);
    printf("Numero de pistas encontradas: %d\n", numPistas);
    if (numPistas >= 2) {
        printf("\nVEREDITO: CULPADO! As provas sao contundentes. Voce solucionou o caso!\n");
    } else {
        printf("\nVEREDITO: INOCENTE! Voce acusou a pessoa errada. O verdadeiro culpado escapou...\n");
    }
}

// --- 5. FUNÇÕES PARA LIBERAR MEMÓRIA (MELHORIA) ---

void liberarTabelaHash() {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashItem *item = tabelaHash[i];
        while (item != NULL) {
            HashItem *temp = item;
            item = item->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

void liberarArvorePistas(NoPista* raiz) {
    if (raiz == NULL) return;
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

void liberarArvoreSalas(Sala* raiz) {
    if (raiz == NULL) return;
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}


// --- 6. LÓGICA PRINCIPAL DO JOGO ---

int main() {
    for (int i = 0; i < HASH_SIZE; i++) {
        tabelaHash[i] = NULL;
    }

    // Montando o mapa da mansão
    Sala* raiz = criarSala("Hall de Entrada", "");
    raiz->esquerda = criarSala("Biblioteca", "Um livro sobre venenos estava fora do lugar.");
    raiz->direita = criarSala("Sala de Jantar", "Um frasco quebrado com um liquido suspeito.");
    raiz->esquerda->esquerda = criarSala("Escritorio", "Uma carta de ameaca enderecada a vitima.");
    raiz->esquerda->direita = criarSala("Jardim de Inverno", "Pegadas de botas sujas de terra.");
    raiz->direita->direita = criarSala("Cozinha", "Uma faca afiada desapareceu do conjunto.");
    raiz->direita->direita->esquerda = criarSala("Despensa", "Um recibo de compra de luvas de couro.");

    // Associando Pistas a Suspeitos
    inserirNaHash("Um livro sobre venenos estava fora do lugar.", "Governanta");
    inserirNaHash("Um frasco quebrado com um liquido suspeito.", "Dama_de_Companhia");
    inserirNaHash("Uma carta de ameaca enderecada a vitima.", "Jardineiro");
    inserirNaHash("Pegadas de botas sujas de terra.", "Jardineiro");
    inserirNaHash("Uma faca afiada desapareceu do conjunto.", "Mordomo");
    inserirNaHash("Um recibo de compra de luvas de couro.", "Mordomo");

    // Início do Jogo
    NoPista* arvoreDePistas = NULL;

    printf("*****************************************\n");
    printf("* DETECTIVE QUEST: O CASO FINAL         *\n");
    printf("*****************************************\n");
    printf("Explore os comodos, colete pistas e descubra o culpado.\n\n");

    explorarSalas(raiz, &arvoreDePistas);
    verificarSuspeitoFinal(arvoreDePistas);

    // --- LIBERANDO TODA A MEMÓRIA ALOCADA ---
    printf("\nLimpando a cena do crime (liberando memoria)...\n");
    liberarTabelaHash();
    liberarArvorePistas(arvoreDePistas);
    liberarArvoreSalas(raiz);
    printf("Sistema finalizado.\n");

    return 0;
}
