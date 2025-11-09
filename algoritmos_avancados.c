#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes e Definições ---
#define TAMANHO_TABELA 7 // Tamanho primo para a Tabela Hash
#define MAX_PISTA_LEN 100
#define MAX_NOME_LEN 50

// --- Estrutura 1: Nó da Árvore de Pistas (BST) ---
typedef struct PistaNode {
    char pista[MAX_PISTA_LEN];  // Conteúdo da pista
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// --- Estrutura 2: Nó da Tabela Hash (Lista Encadeada) ---
typedef struct HashNode {
    char pista[MAX_PISTA_LEN];  // Chave: a pista
    char suspeito[MAX_NOME_LEN]; // Valor: o suspeito associado
    struct HashNode *proximo;
} HashNode;

// Tabela Hash (Array de ponteiros para HashNode)
HashNode *tabelaHash[TAMANHO_TABELA];

// --- Estrutura 3: Nó da Árvore da Mansão (Árvore Binária Simples) ---
typedef struct Sala {
    char nome[MAX_NOME_LEN];
    char pista[MAX_PISTA_LEN]; // Pista estática associada à sala
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;


// --- Protótipos das Funções ---

// Funções da Mansão
Sala* criarSala(const char *nome, const char *pistaConteudo);
void explorarSalas(Sala *salaAtual, PistaNode **raizPistas);

// Funções da BST (Pistas Coletadas)
PistaNode* inserirPista(PistaNode *raiz, const char *novaPista);
void exibirPistas(PistaNode *raiz);

// Funções da Tabela Hash (Associação Pista/Suspeito)
unsigned int hash(const char *chave);
void inserirNaHash(const char *pista, const char *suspeito);
char* encontrarSuspeito(const char *pista);
int contarPistasPorSuspeito(PistaNode *raiz, const char *suspeitoAlvo);

// Funções de Julgamento
void verificarSuspeitoFinal(PistaNode *raizPistas);

// Funções de Limpeza
void liberarPistas(PistaNode *raiz);
void liberarMapa(Sala *raiz);
void liberarHash();


// =============================================================================
// --- IMPLEMENTAÇÃO DA MANSÃO (ÁRVORE BINÁRIA) ---
// =============================================================================

/**
 * @brief Cria dinamicamente uma nova sala (nó da árvore binária) com uma pista estática.
 * @param nome O nome a ser atribuído à nova sala.
 * @param pistaConteudo O conteúdo da pista nesta sala. Use "" se não houver pista.
 * @return Um ponteiro para a Sala recém-criada.
 */
Sala* criarSala(const char *nome, const char *pistaConteudo) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        printf("Erro de alocação de memória para Sala!\n");
        exit(1); 
    }
    
    strncpy(novaSala->nome, nome, MAX_NOME_LEN - 1);
    novaSala->nome[MAX_NOME_LEN - 1] = '\0';
    
    strncpy(novaSala->pista, pistaConteudo, MAX_PISTA_LEN - 1);
    novaSala->pista[MAX_PISTA_LEN - 1] = '\0';

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Permite a navegação interativa do jogador pelo mapa e coleta de pistas,
 * adicionando-as à BST.
 * @param salaAtual O ponteiro para a sala onde o jogador está atualmente.
 * @param raizPistas O ponteiro para o ponteiro da raiz da BST de pistas.
 */
void explorarSalas(Sala *salaAtual, PistaNode **raizPistas) {
    char escolha;
    
    while (salaAtual != NULL) {
        printf("\n======================================================\n");
        printf("✅ Localização Atual: **%s**\n", salaAtual->nome);
        
        // Coleta e armazena a pista se ela existir
        if (strlen(salaAtual->pista) > 0) {
            printf("🔍 **PISTA ENCONTRADA!** Conteúdo: \"%s\"\n", salaAtual->pista);
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            
            // "Limpa" a pista na sala para que não seja coletada novamente
            salaAtual->pista[0] = '\0'; 
            printf("💾 Pista adicionada ao seu Caderno (BST).\n");
        } else {
            printf("... Não há pistas visíveis nesta sala.\n");
        }

        // Interface de navegação
        printf("\nEscolha seu caminho:\n");
        printf("  [e] Esquerda: %s\n", salaAtual->esquerda ? salaAtual->esquerda->nome : "Caminho Bloqueado");
        printf("  [d] Direita: %s\n", salaAtual->direita ? salaAtual->direita->nome : "Caminho Bloqueado");
        printf("  [s] Sair para o Julgamento\n");
        printf("Sua escolha (e/d/s): ");
        
        if (scanf(" %c", &escolha) != 1) {
            escolha = 's';
        }
        while(getchar() != '\n');
        
        // Processa a Escolha
        if (escolha == 's' || escolha == 'S') {
            printf("\n🚪 O detetive decide encerrar a exploração e ir para o julgamento.\n");
            break;
        } else if (escolha == 'e' || escolha == 'E') {
            if (salaAtual->esquerda != NULL) {
                salaAtual = salaAtual->esquerda;
            } else {
                printf("🚫 Caminho da Esquerda Bloqueado! Escolha outra direção.\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (salaAtual->direita != NULL) {
                salaAtual = salaAtual->direita;
            } else {
                printf("🚫 Caminho da Direita Bloqueado! Escolha outra direção.\n");
            }
        } else {
            printf("❌ Opção inválida! Tente novamente.\n");
        }
    }
}

// =============================================================================
// --- IMPLEMENTAÇÃO DA BST (ÁRVORE DE PISTAS) ---
// =============================================================================

/**
 * @brief Insere recursivamente uma nova pista na Árvore Binária de Busca (BST).
 * @param raiz O nó raiz da subárvore atual.
 * @param novaPista A string contendo a pista a ser inserida.
 * @return O ponteiro para a raiz atualizada da subárvore.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *novaPista) {
    if (raiz == NULL) {
        PistaNode *novoNode = (PistaNode*)malloc(sizeof(PistaNode));
        if (novoNode == NULL) {
             printf("Erro de alocação de memória para PistaNode!\n");
             exit(1);
        }
        strncpy(novoNode->pista, novaPista, MAX_PISTA_LEN - 1);
        novoNode->pista[MAX_PISTA_LEN - 1] = '\0';
        novoNode->esquerda = NULL;
        novoNode->direita = NULL;
        return novoNode;
    }

    // Compara a nova pista com a pista do nó atual
    int comparacao = strcasecmp(novaPista, raiz->pista);

    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, novaPista);
    } 
    // Ignora duplicatas

    return raiz;
}

/**
 * @brief Exibe todas as pistas coletadas em ordem alfabética (percurso Em Ordem).
 * @param raiz O nó raiz da BST de pistas.
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// =============================================================================
// --- IMPLEMENTAÇÃO DA TABELA HASH (ASSOCIAÇÃO PISTA-SUSPEITO) ---
// =============================================================================

/**
 * @brief Função hash simples (soma dos valores ASCII módulo o tamanho).
 * @param chave A string (pista) a ser hasheada.
 * @return O índice na tabela hash.
 */
unsigned int hash(const char *chave) {
    unsigned int valor = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        valor = valor * 31 + chave[i];
    }
    return valor % TAMANHO_TABELA;
}

/**
 * @brief Insere a associação pista/suspeito na tabela hash, tratando colisões por encadeamento.
 * @param pista A string da pista (chave).
 * @param suspeito A string do suspeito (valor).
 */
void inserirNaHash(const char *pista, const char *suspeito) {
    unsigned int indice = hash(pista);

    // Cria o novo nó
    HashNode *novoNode = (HashNode*)malloc(sizeof(HashNode));
    if (novoNode == NULL) {
        printf("Erro de alocação de memória para HashNode!\n");
        exit(1);
    }
    strncpy(novoNode->pista, pista, MAX_PISTA_LEN - 1);
    novoNode->pista[MAX_PISTA_LEN - 1] = '\0';
    strncpy(novoNode->suspeito, suspeito, MAX_NOME_LEN - 1);
    novoNode->suspeito[MAX_NOME_LEN - 1] = '\0';
    novoNode->proximo = NULL;

    // Inserção no início da lista encadeada (encadeamento separado)
    novoNode->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNode;
}

/**
 * @brief Consulta o suspeito correspondente a uma pista na tabela hash.
 * @param pista A pista (chave) a ser procurada.
 * @return A string do nome do suspeito, ou NULL se a pista não for encontrada.
 */
char* encontrarSuspeito(const char *pista) {
    unsigned int indice = hash(pista);
    HashNode *atual = tabelaHash[indice];

    // Percorre a lista encadeada no índice
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito; // Encontrado!
        }
        atual = atual->proximo;
    }
    return NULL; // Não encontrado
}

/**
 * @brief Percorre a BST de pistas coletadas e conta quantas apontam para um suspeito alvo.
 * Esta função usa a Tabela Hash para fazer a consulta.
 * @param raiz O nó raiz da BST de pistas coletadas.
 * @param suspeitoAlvo O nome do suspeito a ser contado.
 * @return O número de pistas coletadas que apontam para o suspeito alvo.
 */
int contarPistasPorSuspeito(PistaNode *raiz, const char *suspeitoAlvo) {
    if (raiz == NULL) {
        return 0;
    }
    
    int contagem = 0;
    
    // 1. Visita o nó atual: consulta o suspeito na Tabela Hash
    char *suspeitoDaPista = encontrarSuspeito(raiz->pista);
    if (suspeitoDaPista != NULL && strcasecmp(suspeitoDaPista, suspeitoAlvo) == 0) {
        contagem = 1; // Esta pista aponta para o suspeito
    }
    
    // 2. Visita subárvores recursivamente
    contagem += contarPistasPorSuspeito(raiz->esquerda, suspeitoAlvo);
    contagem += contarPistasPorSuspeito(raiz->direita, suspeitoAlvo);
    
    return contagem;
}


// =============================================================================
// --- IMPLEMENTAÇÃO DA LÓGICA DE JULGAMENTO FINAL ---
// =============================================================================

/**
 * @brief Conduz à fase de julgamento, solicitando a acusação e avaliando a evidência.
 * @param raizPistas A raiz da BST contendo todas as pistas coletadas.
 */
void verificarSuspeitoFinal(PistaNode *raizPistas) {
    char acusacao[MAX_NOME_LEN];
    int pistasNecessarias = 2;

    if (raizPistas == NULL) {
        printf("\n\n*** JULGAMENTO FINAL ***\n");
        printf("⚠️ Você não coletou nenhuma pista. Não é possível fazer uma acusação!\n");
        return;
    }
    
    // 1. Solicita a Acusação
    printf("\n\n*** JULGAMENTO FINAL ***\n");
    printf("\nSuspeitos principais: Mordomo, Jardineiro, Dona da Casa, Cozinheira.\n");
    printf("Com base nas suas pistas, quem você acusa? (Nome): ");
    
    // Garante que o nome tenha espaço para ser lido e limpa o buffer antes
    if (scanf(" %49[^\n]", acusacao) != 1) { 
        strcpy(acusacao, "Ninguém");
    }
    while(getchar() != '\n');
    
    printf("\n⚖️ Acusação formalizada contra: **%s**.\n", acusacao);

    // 2. Avaliação das Evidências
    printf("Avaliando as evidências coletadas...\n");
    int pistasEncontradas = contarPistasPorSuspeito(raizPistas, acusacao);
    
    // 3. Desfecho
    if (pistasEncontradas >= pistasNecessarias) {
        printf("\n🎉 SUCESSO! **%s** é o culpado!\n", acusacao);
        printf("Havia %d pistas que ligavam %s ao crime. Justiça feita!\n", 
               pistasEncontradas, acusacao);
    } else {
        printf("\n❌ FRACASSO. A acusação contra %s não pôde ser sustentada.\n", acusacao);
        printf("Você só encontrou %d das %d pistas necessárias para condená-lo(a).\n", 
               pistasEncontradas, pistasNecessarias);
    }
}


// =============================================================================
// --- FUNÇÃO PRINCIPAL E MONTAGEM INICIAL ---
// =============================================================================

int main() {
    printf("--- Detective Quest: Nível Mestre (Hash e Julgamento) ---\n");

    // Inicializa a BST de Pistas (Caderno do Detetive)
    PistaNode *raizPistas = NULL;
    
    // Inicializa a Tabela Hash
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabelaHash[i] = NULL;
    }
    
    // 1. Montagem da Tabela Hash (Associações Pista -> Suspeito)
    printf("\n⚙️ Mapeando as associações Pista -> Suspeito na Tabela Hash...\n");
    // Pistas que ligam ao MORDOMO (4 pistas)
    inserirNaHash("A arma do crime estava no Quarto Principal", "Mordomo");
    inserirNaHash("O culpado é um especialista em venenos", "Mordomo");
    inserirNaHash("Um recibo de passagem para a França", "Mordomo");
    inserirNaHash("A carta de chantagem estava na Sala de Música", "Mordomo");
    
    // Pistas que ligam ao JARDINEIRO (2 pistas)
    inserirNaHash("Pegadas de bota na Cozinha", "Jardineiro");
    inserirNaHash("Tesoura de poda encontrada no Escritório", "Jardineiro");

    // Pistas que ligam à DONA DA CASA (2 pistas)
    inserirNaHash("A joia roubada sumiu da Biblioteca", "Dona da Casa");
    inserirNaHash("Um bilhete secreto na Sala de Estar", "Dona da Casa");

    // Pistas que ligam à COZINHEIRA (2 pistas)
    inserirNaHash("O avental estava sujo de lama", "Cozinheira");
    inserirNaHash("Falta uma faca especial na Dispensa", "Cozinheira");

    // 2. Montagem da Árvore Binária (Mapa da Mansão) com Pistas Estáticas
    
    // Nível 0 (Raiz)
    Sala *hallEntrada = criarSala("Hall de Entrada", "A joia roubada sumiu da Biblioteca"); 
    
    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar", "O culpado é um especialista em venenos");
    Sala *cozinha = criarSala("Cozinha", "O avental estava sujo de lama");
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    
    // Nível 2
    Sala *biblioteca = criarSala("Biblioteca", "Pegadas de bota na Cozinha");
    Sala *salaJantar = criarSala("Sala de Jantar", "A arma do crime estava no Quarto Principal");
    Sala *escritorio = criarSala("Escritório", ""); // Sem Pista
    Sala *jardim = criarSala("Jardim", "Um recibo de passagem para a França");
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = salaJantar;
    
    cozinha->esquerda = escritorio;
    cozinha->direita = jardim;

    // Nível 3 (Caminhos Finais)
    biblioteca->esquerda = criarSala("Quarto Principal", "A carta de chantagem estava na Sala de Música");
    
    salaJantar->esquerda = criarSala("Dispensa", "Falta uma faca especial na Dispensa");
    salaJantar->direita = criarSala("Porão", "Tesoura de poda encontrada no Escritório");
    
    escritorio->esquerda = criarSala("Sala de Música", "Um bilhete secreto na Sala de Estar");

    // 3. Início da Exploração
    printf("\n🗺️ O mapa da mansão foi montado. Inicie sua investigação!\n");
    explorarSalas(hallEntrada, &raizPistas);
    
    // 4. Exibição das Pistas e Julgamento
    printf("\n\n*** PISTAS COLETADAS (EM ORDEM ALFABÉTICA) ***\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(raizPistas);
    }
    printf("************************************************\n");

    // Chama a função principal do Desafio Nível Mestre
    verificarSuspeitoFinal(raizPistas);

    // 5. Liberação de Memória
    liberarPistas(raizPistas);
    liberarMapa(hallEntrada);
    liberarHash();
    
    printf("\n--- Fim do Programa. Memória das estruturas liberada. ---\n");
    return 0;
}

// -----------------------------------------------------------------------------
// Funções de Liberação de Memória (Boa Prática)
// -----------------------------------------------------------------------------
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

void liberarMapa(Sala *raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

void liberarHash() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        HashNode *atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}