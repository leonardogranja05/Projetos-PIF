#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define LARGURA 30
#define ALTURA 20
#define GRAVIDADE 1
#define PULO -3
#define ESPINHOS_FREQUENCIA 5

int nave_x = 5, nave_y = ALTURA / 2;
int espinhos_x[LARGURA];
int espinhos_y[LARGURA];
int pontuacao = 0;

void inicializar() {
    srand(time(0));
    for (int i = 0; i < LARGURA; i++) {
        espinhos_x[i] = LARGURA + i * ESPINHOS_FREQUENCIA;
        espinhos_y[i] = rand() % ALTURA;
    }
}

void desenhar() {
    system("cls");

    for (int y = 0; y < ALTURA; y++) {
        for (int x = 0; x < LARGURA; x++) {
            if (x == nave_x && y == nave_y) {
                printf("A");
            } else {
                int ehEspinho = 0;
                for (int i = 0; i < LARGURA; i++) {
                    if (x == espinhos_x[i] && y == espinhos_y[i]) {
                        printf("^");
                        ehEspinho = 1;
                        break;
                    }
                }
                if (!ehEspinho) printf(" ");
            }
        }
        printf("\n");
    }
    printf("Pontuação: %d\n", pontuacao);
}

void atualizar() {
    nave_y += GRAVIDADE;

    for (int i = 0; i < LARGURA; i++) {
        espinhos_x[i]--;
        if (espinhos_x[i] < 0) {
            espinhos_x[i] = LARGURA;
            espinhos_y[i] = rand() % ALTURA;
            pontuacao++;
        }
        if (nave_x == espinhos_x[i] && nave_y == espinhos_y[i]) {
            printf("Game Over! Sua pontuação foi: %d\n", pontuacao);
            exit(0);
        }
    }

    if (nave_y < 0 || nave_y >= ALTURA) {
        printf("Game Over! Sua pontuação foi: %d\n", pontuacao);
        exit(0);
    }
}

void jogar() {
    inicializar();
    while (1) {
        desenhar();
        atualizar();
        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == ' ') nave_y += PULO;
        }
        Sleep(100);
    }
}

int main() {
    printf("Bem-vindo ao Jogo da Nave e Espinhos!\n");
    printf("Pressione ESPAÇO para fazer a nave subir.\n");
    printf("Pressione qualquer tecla para começar...\n");
    _getch();
    jogar();
    return 0;
}
