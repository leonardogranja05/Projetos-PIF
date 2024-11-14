#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

#define GRAVIDADE 1
#define PULO -3
#define ESPINHOS_FREQUENCIA 5

typedef struct {
    int x;
    int y;
} Nave;

typedef struct {
    int x;
    int y;
} Espinho;

typedef struct {
    Nave nave;
    Espinho* espinhos;
    int pontuacao;
    int largura;
    int altura;
    int num_espinhos;
} Jogo;

void inicializar(Jogo* jogo) {
    srand(time(0));

    jogo->nave.x = 5;
    jogo->nave.y = jogo->altura / 2;

    jogo->espinhos = (Espinho*)malloc(jogo->num_espinhos * sizeof(Espinho));
    for (int i = 0; i < jogo->num_espinhos; i++) {
        jogo->espinhos[i].x = jogo->largura + i * ESPINHOS_FREQUENCIA;
        jogo->espinhos[i].y = rand() % jogo->altura;
    }

    jogo->pontuacao = 0;
}

void desenhar(Jogo* jogo) {
    system("clear");

    for (int y = 0; y < jogo->altura; y++) {
        for (int x = 0; x < jogo->largura; x++) {
            if (x == jogo->nave.x && y == jogo->nave.y) {
                printf("A");  
            } else {
                int ehEspinho = 0;
                for (int i = 0; i < jogo->num_espinhos; i++) {
                    if (x == jogo->espinhos[i].x && y == jogo->espinhos[i].y) {
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
    printf("Pontuação: %d\n", jogo->pontuacao);
}

void atualizar(Jogo* jogo) {
    jogo->nave.y += GRAVIDADE;

    for (int i = 0; i < jogo->num_espinhos; i++) {
        jogo->espinhos[i].x--;
        if (jogo->espinhos[i].x < 0) {
            jogo->espinhos[i].x = jogo->largura;
            jogo->espinhos[i].y = rand() % jogo->altura;
            jogo->pontuacao++;
        }
        if (jogo->nave.x == jogo->espinhos[i].x && jogo->nave.y == jogo->espinhos[i].y) {
            printf("Game Over! Sua pontuação foi: %d\n", jogo->pontuacao);
            free(jogo->espinhos);
            free(jogo);
            exit(0);
        }
    }

    if (jogo->nave.y < 0 || jogo->nave.y >= jogo->altura) {
        printf("Game Over! Sua pontuação foi: %d\n", jogo->pontuacao);
        free(jogo->espinhos); 
        free(jogo); 
        exit(0);
    }
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

char getch(void) {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void jogar(Jogo* jogo) {
    inicializar(jogo);
    while (1) {
        desenhar(jogo);
        atualizar(jogo);
        if (kbhit()) {
            char tecla = getch();
            if (tecla == ' ') jogo->nave.y += PULO; 
        }
        usleep(100000); 
    }
}

int main() {
    Jogo* jogo = (Jogo*)malloc(sizeof(Jogo));
    if (!jogo) {
        fprintf(stderr, "Erro ao alocar memória para o jogo.\n");
        return 1;
    }

    jogo->largura = 30;
    jogo->altura = 20;
    jogo->num_espinhos = 30;

    printf("Bem-vindo ao Jogo da Nave e Espinhos!\n");
    printf("Pressione ESPAÇO para fazer a nave subir.\n");
    printf("Pressione qualquer tecla para começar...\n");
    getch();
    jogar(jogo);

    free(jogo->espinhos);
    free(jogo);

    return 0;
}
