#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura para representar um cômodo/sala na mansão
typedef struct Sala {
    char nome[50];       // Nome do cômodo
    struct Sala *esquerda; // Ponteiro para o cômodo à esquerda
    struct Sala *direita;  // Ponteiro para o cômodo à direita
} Sala;

// --- Protótipos das Funções ---
Sala* criarSala(const char *nome);
void explorarSalas(Sala *salaAtual);

// -----------------------------------------------------------------------------
/**
 * @brief Cria dinamicamente uma nova sala (nó da árvore binária).
 * * @param nome O nome a ser atribuído à nova sala.
 * @return Um ponteiro para a Sala recém-criada, ou NULL se a alocação falhar.
 */
Sala* criarSala(const char *nome) {
    // Aloca memória para a nova sala
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    
    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro de alocação de memória!\n");
        exit(1); // Sai do programa em caso de falha crítica
    }
    
    // Copia o nome para o campo 'nome' da estrutura
    strncpy(novaSala->nome, nome, 49);
    novaSala->nome[49] = '\0'; // Garante terminação da string
    
    // Inicializa os ponteiros dos filhos como NULL (sem caminhos inicialmente)
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

// -----------------------------------------------------------------------------
/**
 * @brief Permite a navegação interativa do jogador pela árvore binária (mapa).
 * * @param salaAtual O ponteiro para a sala onde o jogador está atualmente.
 */
void explorarSalas(Sala *salaAtual) {
    char escolha;
    
    while (salaAtual != NULL) {
        // Exibe o nome da sala atual
        printf("\n✅ Você está em: **%s**\n", salaAtual->nome);
        
        // Verifica se é um nó-folha (não há caminhos de saída)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("⚠️ Não há mais caminhos a seguir nesta sala. A exploração termina aqui!\n");
            break; // Sai do loop e encerra a exploração
        }

        // --- Interface de Navegação ---
        printf("\nEscolha seu caminho:\n");
        
        // Exibe opções baseadas nos caminhos disponíveis
        if (salaAtual->esquerda != NULL) {
            printf("  [e] Esquerda (para a sala: %s)\n", salaAtual->esquerda->nome);
        } else {
            printf("  [e] Esquerda (Caminho Bloqueado)\n");
        }
        
        if (salaAtual->direita != NULL) {
            printf("  [d] Direita (para a sala: %s)\n", salaAtual->direita->nome);
        } else {
            printf("  [d] Direita (Caminho Bloqueado)\n");
        }
        
        printf("  [s] Sair do Jogo\n");
        printf("Sua escolha (e/d/s): ");
        
        // Lê a escolha do jogador
        if (scanf(" %c", &escolha) != 1) {
            // Caso a leitura falhe (ex: EOF), considera 's' para sair
            escolha = 's';
        }
        
        // Limpa o buffer de entrada para evitar problemas na próxima leitura
        while(getchar() != '\n'); 
        
        // --- Processa a Escolha ---
        if (escolha == 's' || escolha == 'S') {
            printf("\n🚪 O jogador decidiu sair. Exploração encerrada.\n");
            break;
        } else if (escolha == 'e' || escolha == 'E') {
            if (salaAtual->esquerda != NULL) {
                // Move para a sala da esquerda
                salaAtual = salaAtual->esquerda;
            } else {
                printf("🚫 Caminho da Esquerda Bloqueado! Escolha outra direção.\n");
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (salaAtual->direita != NULL) {
                // Move para a sala da direita
                salaAtual = salaAtual->direita;
            } else {
                printf("🚫 Caminho da Direita Bloqueado! Escolha outra direção.\n");
            }
        } else {
            printf("❌ Opção inválida! Por favor, escolha 'e', 'd' ou 's'.\n");
        }
    }
}

// -----------------------------------------------------------------------------
/**
 * @brief Função principal. Monta o mapa inicial da mansão e inicia a exploração.
 */
int main() {
    printf("--- Detective Quest: Exploração da Mansão Enigma Studios ---\n");

    // 1. Montagem da Árvore Binária (Mapa da Mansão)
    
    // Nível 0 (Raiz)
    Sala *hallEntrada = criarSala("Hall de Entrada"); // Raiz da árvore
    
    // Nível 1
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    
    // Nível 2
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *salaJantar = criarSala("Sala de Jantar");
    Sala *escritorio = criarSala("Escritório");
    Sala *jardim = criarSala("Jardim");
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = salaJantar;
    
    cozinha->esquerda = escritorio;
    cozinha->direita = jardim;

    // Nível 3 (Nós-Folha - Fim do Caminho)
    biblioteca->esquerda = criarSala("Quarto Principal");
    // biblioteca->direita continua NULL, é um nó-folha, mas com um caminho
    
    salaJantar->esquerda = criarSala("Dispensa");
    salaJantar->direita = criarSala("Porão");
    
    escritorio->esquerda = criarSala("Quarto de Hóspedes");
    // escritorio->direita continua NULL
    
    // jardim->esquerda e jardim->direita continuam NULL - nó-folha sem mais caminhos

    // 2. Início da Exploração
    printf("\n🗺️ O mapa da mansão foi montado. Inicie sua jornada no Hall de Entrada!\n");
    explorarSalas(hallEntrada);

    // 3. Liberação de Memória (Boa Prática)
    // Para simplificar o nível novato e focar na exploração, 
    // a liberação de memória dos nós alocados (usando uma função de percorrimento pós-ordem)
    // é omitida, mas em um programa real, seria essencial.
    // Exemplo de como a liberação seria feita:
    /*
    void liberarMapa(Sala *raiz) {
        if (raiz != NULL) {
            liberarMapa(raiz->esquerda);
            liberarMapa(raiz->direita);
            free(raiz);
        }
    }
    liberarMapa(hallEntrada);
    */

    printf("\n--- Fim do Jogo. Obrigado por jogar Detective Quest! ---\n");
    return 0;
}