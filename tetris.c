#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char tipo;  // tipo da peça (ex: 'I', 'O', 'T', 'S', 'Z', 'L', 'J')
    int id;     // identificador único
} Peca;

// --------------------- FILA CIRCULAR ---------------------
typedef struct {
    Peca vetor[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->vetor[f->tras] = p;
    f->tamanho++;
}

Peca dequeue(Fila *f) {
    Peca invalida = {'-', -1};
    if (filaVazia(f)) return invalida;
    Peca p = f->vetor[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->tamanho--;
    return p;
}

Peca frenteFila(Fila *f) {
    return filaVazia(f) ? (Peca){'-', -1} : f->vetor[f->frente];
}

void mostrarFila(Fila *f) {
    printf("\n[FILA] Peças futuras:\n");
    if (filaVazia(f)) {
        printf("Fila vazia\n");
        return;
    }
    int i = f->frente;
    for (int c = 0; c < f->tamanho; c++) {
        printf(" (%d | %c) ", f->vetor[i].id, f->vetor[i].tipo);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

// --------------------- PILHA ---------------------
typedef struct {
    Peca vetor[TAM_PILHA];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) return;
    p->vetor[++p->topo] = x;
}

Peca pop(Pilha *p) {
    return pilhaVazia(p) ? (Peca){'-', -1} : p->vetor[p->topo--];
}

Peca topoPilha(Pilha *p) {
    return pilhaVazia(p) ? (Peca){'-', -1} : p->vetor[p->topo];
}

void mostrarPilha(Pilha *p) {
    printf("[PILHA] Reserva:\n");
    if (pilhaVazia(p)) {
        printf("Pilha vazia\n");
        return;
    }
    for (int i = 0; i <= p->topo; i++) {
        printf(" (%d | %c) ", p->vetor[i].id, p->vetor[i].tipo);
    }
    printf("\n");
}

// --------------------- GERAÇÃO DE PEÇA ---------------------
char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'L', 'J'};

Peca gerarPeca(int *contadorId) {
    Peca p;
    p.tipo = tipos[rand() % 7];
    p.id = (*contadorId)++;
    return p;
}

// --------------------- TROCAS AVANÇADAS ---------------------
void trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("❌ Impossível trocar: estruturas vazias.\n");
        return;
    }
    int idx = f->frente;
    Peca temp = f->vetor[idx];
    f->vetor[idx] = p->vetor[p->topo];
    p->vetor[p->topo] = temp;
    printf("✅ Peça da frente da fila trocada com o topo da pilha.\n");
}

void trocarTresPrimeiros(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo != 2) {
        printf("❌ Impossível trocar 3 peças: tamanho insuficiente.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idxFila = (f->frente + i) % TAM_FILA;
        Peca temp = f->vetor[idxFila];
        f->vetor[idxFila] = p->vetor[i];
        p->vetor[i] = temp;
    }
    printf("✅ As 3 primeiras peças da fila foram trocadas com a pilha.\n");
}

// --------------------- MENU PRINCIPAL ---------------------
void exibirMenu() {
    printf("\n========= MENU =========\n");
    printf("1 - Jogar peça (remover da fila)\n");
    printf("2 - Enviar peça da fila para reserva (pilha)\n");
    printf("3 - Usar peça da reserva (topo da pilha → fila)\n");
    printf("4 - Trocar frente da fila com topo da pilha\n");
    printf("5 - Trocar 3 primeiros da fila com 3 da pilha\n");
    printf("0 - Sair\n");
    printf("========================\n");
}

int main() {
    Fila fila;
    Pilha pilha;
    int contadorId = 1;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preencher fila inicialmente
    while (!filaCheia(&fila)) {
        enqueue(&fila, gerarPeca(&contadorId));
    }

    do {
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        exibirMenu();
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = dequeue(&fila);
                if (jogada.id == -1)
                    printf("⚠️ Fila vazia.\n");
                else {
                    printf("🎮 Peça jogada: (%d | %c)\n", jogada.id, jogada.tipo);
                    enqueue(&fila, gerarPeca(&contadorId));
                }
                break;
            }
            case 2: {
                if (pilhaCheia(&pilha)) {
                    printf("❌ Pilha cheia.\n");
                } else if (filaVazia(&fila)) {
                    printf("⚠️ Fila vazia.\n");
                } else {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    enqueue(&fila, gerarPeca(&contadorId));
                    printf("📦 Peça (%d | %c) enviada para reserva.\n", p.id, p.tipo);
                }
                break;
            }
            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("⚠️ Nenhuma peça na reserva.\n");
                } else if (filaCheia(&fila)) {
                    printf("⚠️ Fila cheia. Não é possível adicionar peça.\n");
                } else {
                    Peca p = pop(&pilha);
                    enqueue(&fila, p);
                    printf("✅ Peça da reserva usada: (%d | %c)\n", p.id, p.tipo);
                }
                break;
            }
            case 4:
                trocarFrenteComTopo(&fila, &pilha);
                break;
            case 5:
                trocarTresPrimeiros(&fila, &pilha);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("❌ Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}
