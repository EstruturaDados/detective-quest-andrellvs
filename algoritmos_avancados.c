#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definição das Estruturas ---

// Estrutura para representar um nó na ÁRVORE DE PISTAS (BST)
typedef struct PistaNode {
    char pista[100];            // Conteúdo da pista
    struct PistaNode *esquerda; // Pistas menores/anteriores (alfabeticamente)
    struct PistaNode *direita;  // Pistas maiores/posteriores (alfabeticamente)
} PistaNode;

// Estrutura para representar um cômodo/sala na MANSÃO (Árvore Binária Simples)
typedef struct Sala {
    char nome[50];              // Nome do cômodo
    char pista[100];            // Pista associada a este cômodo (pode ser vazia)
    struct Sala *esquerda;      // Ponteiro para o cômodo à esquerda
    struct Sala *direita;       // Ponteiro para o cômodo à direita
} Sala;

// --- Protótipos das Funções ---
Sala* criarSala(const char *nome, const char *pistaConteudo);
PistaNode* inserirPista(PistaNode *raiz, const char *novaPista);
void explorarSalasComPistas(Sala *salaAtual, PistaNode **raizPistas);
void exibirPistas(PistaNode *raiz);
void liberarPistas(PistaNode *raiz); // Para liberar memória

// -----------------------------------------------------------------------------
/**
 * @brief Cria dinamicamente uma nova sala (nó da árvore binária) com uma pista opcional.
 * @param nome O nome a ser atribuído à nova sala.
 * @param pistaConteudo O conteúdo da pista nesta sala. Use "" se não houver pista.
 * @return Um ponteiro para a Sala recém-criada.
 */
Sala* criarSala(const char *nome, const char *pistaConteudo) {
    // Aloca memória para a nova sala
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        printf("Erro de alocação de memória para Sala!\n");
        exit(1); 
    }
    
    // Inicializa campos
    strncpy(novaSala->nome, nome, 49);
    novaSala->nome[49] = '\0';
    
    strncpy(novaSala->pista, pistaConteudo, 99);
    novaSala->pista[99] = '\0';

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

// -----------------------------------------------------------------------------
/**
 * @brief Insere recursivamente uma nova pista na Árvore Binária de Busca (BST).
 * @param raiz O nó raiz da subárvore atual.
 * @param novaPista A string contendo a pista a ser inserida.
 * @return O ponteiro para a raiz atualizada da subárvore.
 */
PistaNode* inserirPista(PistaNode *raiz, const char *novaPista) {
    // Caso base: Se a subárvore estiver vazia, cria o novo nó e retorna
    if (raiz == NULL) {
        PistaNode *novoNode = (PistaNode*)malloc(sizeof(PistaNode));
        if (novoNode == NULL) {
             printf("Erro de alocação de memória para PistaNode!\n");
             exit(1);
        }
        strncpy(novoNode->pista, novaPista, 99);
        novoNode->pista[99] = '\0';
        novoNode->esquerda = NULL;
        novoNode->direita = NULL;
        return novoNode;
    }

    // Compara a nova pista com a pista do nó atual (ignorando maiúsculas/minúsculas)
    int comparacao = strcasecmp(novaPista, raiz->pista);

    if (comparacao < 0) {
        // Nova pista é alfabeticamente menor -> Insere na subárvore esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (comparacao > 0) {
        // Nova pista é alfabeticamente maior -> Insere na subárvore direita
        raiz->direita = inserirPista(raiz->direita, novaPista);
    } 
    // Se comparacao == 0, a pista já existe; não insere duplicata.

    return raiz;
}

// -----------------------------------------------------------------------------
/**
 * @brief Permite a navegação interativa do jogador pelo mapa e coleta de pistas.
 * @param salaAtual O ponteiro para a sala onde o jogador está atualmente.
 * @param raizPistas O ponteiro para o ponteiro da raiz da BST de pistas (para modificação).
 */
void explorarSalasComPistas(Sala *salaAtual, PistaNode **raizPistas) {
    char escolha;
    
    while (salaAtual != NULL) {
        printf("\n======================================================\n");
        printf("✅ Localização Atual: **%s**\n", salaAtual->nome);
        
        // Verifica e coleta a pista
        if (strlen(salaAtual->pista) > 0) {
            printf("🔍 **PISTA ENCONTRADA!** Conteúdo: \"%s\"\n", salaAtual->pista);
            *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
            
            // "Limpa" a pista na sala para que não seja coletada novamente
            salaAtual->pista[0] = '\0'; 
            printf("💾 Pista adicionada ao seu caderno (BST).\n");
        } else {
            printf("... Não há pistas visíveis nesta sala.\n");
        }

        // Exibe opções de navegação
        printf("\nEscolha seu caminho:\n");
        printf("  [e] Esquerda: %s\n", salaAtual->esquerda ? salaAtual->esquerda->nome : "Caminho Bloqueado");
        printf("  [d] Direita: %s\n", salaAtual->direita ? salaAtual->direita->nome : "Caminho Bloqueado");
        printf("  [s] Sair da Mansão\n");
        printf("Sua escolha (e/d/s): ");
        
        if (scanf(" %c", &escolha) != 1) {
            escolha = 's'; // Sair em caso de erro de leitura
        }
        while(getchar() != '\n'); // Limpa o buffer de entrada
        
        // Processa a Escolha
        if (escolha == 's' || escolha == 'S') {
            printf("\n🚪 O detetive decide sair da mansão. Exploração encerrada.\n");
            break;
        } else if (escolha == 'e' || escolha == 'E') {
            if (salaAtual->esquerda != NULL) {
                salaAtual = salaAtual->esquerda;
            } else {
                printf("🚫 Caminho da Esquerda Bloqueado!\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (salaAtual->direita != NULL) {
                salaAtual = salaAtual->direita;
            } else {
                printf("🚫 Caminho da Direita Bloqueado!\n");
            }
        } else {
            printf("❌ Opção inválida! Tente novamente.\n");
        }
    }
}

// -----------------------------------------------------------------------------
/**
 * @brief Exibe todas as pistas coletadas em ordem alfabética (percurso Em Ordem/Inorder da BST).
 * @param raiz O nó raiz da BST de pistas.
 */
void exibirPistas(PistaNode *raiz) {
    // Se for NULL, simplesmente retorna (caso base da recursão)
    if (raiz != NULL) {
        // 1. Visita a subárvore esquerda (menores/anteriores)
        exibirPistas(raiz->esquerda);
        
        // 2. Visita o nó atual (imprime a pista)
        printf("- %s\n", raiz->pista);
        
        // 3. Visita a subárvore direita (maiores/posteriores)
        exibirPistas(raiz->direita);
    }
}

// -----------------------------------------------------------------------------
// Função auxiliar para liberar a memória da BST de pistas (Boa Prática)
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}
// Função auxiliar para liberar a memória do mapa (Boa Prática)
void liberarMapa(Sala *raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}
// -----------------------------------------------------------------------------

/**
 * @brief Função principal: monta o mapa e a BST de pistas, e inicia a exploração.
 */
int main() {
    printf("--- Detective Quest: Nível Aventureiro (Coleta de Pistas) ---\n");

    // Inicializa a BST de Pistas (Caderno do Detetive)
    PistaNode *raizPistas = NULL;
    
    // 1. Montagem da Árvore Binária (Mapa da Mansão) com Pistas
    // Usamos "" para salas sem pista
    
    // Nível 0 (Raiz)
    Sala *hallEntrada = criarSala("Hall de Entrada", "O culpado tem medo de escuro"); 
    
    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar", "");
    Sala *cozinha = criarSala("Cozinha", "A chave mestra está no Jardim");
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    
    // Nível 2
    Sala *biblioteca = criarSala("Biblioteca", "O crime aconteceu à meia-noite");
    Sala *salaJantar = criarSala("Sala de Jantar", "");
    Sala *escritorio = criarSala("Escritório", "Há um bilhete na Dispensa");
    Sala *jardim = criarSala("Jardim", "As janelas estavam abertas");
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = salaJantar;
    
    cozinha->esquerda = escritorio;
    cozinha->direita = jardim;

    // Nível 3 (Caminhos Finais)
    biblioteca->esquerda = criarSala("Quarto Principal", "O mordomo é canhoto");
    
    salaJantar->esquerda = criarSala("Dispensa", "O livro de Agatha Christie");
    salaJantar->direita = criarSala("Porão", ""); // Sem pista
    
    escritorio->esquerda = criarSala("Quarto de Hóspedes", "Um anel foi perdido");

    // 2. Início da Exploração
    printf("\n🗺️ Mapa da mansão montado. Comece sua investigação!\n");
    explorarSalasComPistas(hallEntrada, &raizPistas);

    // 3. Exibição Final das Pistas Coletadas (Em Ordem Alfabética)
    printf("\n\n*** CADERNO DE PISTAS DO DETETIVE ***\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada durante a exploração.\n");
    } else {
        printf("Pistas coletadas (em ordem alfabética):\n");
        exibirPistas(raizPistas);
    }
    printf("****************************************\n");

    // 4. Liberação de Memória
    liberarPistas(raizPistas);
    liberarMapa(hallEntrada);
    
    printf("\n--- Fim do Programa. Memória liberada. ---\n");
    return 0;
}