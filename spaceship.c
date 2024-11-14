// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h> // Para usar usleep no lugar de Sleep no macOS
// #include <time.h>   // Para controle de tempo e geração de números aleatórios
// #include <termios.h> // Para configurações de terminal (substitui _getch() e _kbhit)
// #include <fcntl.h>

// #define LARGURA 30
// #define ALTURA 20
// #define GRAVIDADE 1
// #define PULO -3
// #define ESPINHOS_FREQUENCIA 5

// // Posições da nave
// int nave_x = 5, nave_y = ALTURA / 2;

// // Posição dos espinhos
// int espinhos_x[LARGURA];
// int espinhos_y[LARGURA];

// int pontuacao = 0;

// // Função para inicializar o jogo
// void inicializar() {
//     srand(time(0));
//     for (int i = 0; i < LARGURA; i++) {
//         espinhos_x[i] = LARGURA + i * ESPINHOS_FREQUENCIA;
//         espinhos_y[i] = rand() % ALTURA;
//     }
// }

// // Função para desenhar o jogo
// void desenhar() {
//     system("clear"); // Usar "clear" em vez de "cls" no macOS/Linux

//     for (int y = 0; y < ALTURA; y++) {
//         for (int x = 0; x < LARGURA; x++) {
//             if (x == nave_x && y == nave_y) {
//                 printf("A");  // A nave
//             } else {
//                 int ehEspinho = 0;
//                 for (int i = 0; i < LARGURA; i++) {
//                     if (x == espinhos_x[i] && y == espinhos_y[i]) {
//                         printf("^");  // Espinho
//                         ehEspinho = 1;
//                         break;
//                     }
//                 }
//                 if (!ehEspinho) printf(" ");
//             }
//         }
//         printf("\n");
//     }
//     printf("Pontuação: %d\n", pontuacao);
// }

// // Função para atualizar o estado do jogo
// void atualizar() {
//     nave_y += GRAVIDADE;

//     for (int i = 0; i < LARGURA; i++) {
//         espinhos_x[i]--;
//         if (espinhos_x[i] < 0) {
//             espinhos_x[i] = LARGURA;
//             espinhos_y[i] = rand() % ALTURA;
//             pontuacao++;
//         }
//         if (nave_x == espinhos_x[i] && nave_y == espinhos_y[i]) {
//             printf("Game Over! Sua pontuação foi: %d\n", pontuacao);
//             exit(0);
//         }
//     }

//     if (nave_y < 0 || nave_y >= ALTURA) {
//         printf("Game Over! Sua pontuação foi: %d\n", pontuacao);
//         exit(0);
//     }
// }

// // Funções para simular _kbhit() e _getch() no macOS/Linux
// int kbhit(void) {
//     struct termios oldt, newt;
//     int ch;
//     int oldf;

//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//     fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

//     ch = getchar();

//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     fcntl(STDIN_FILENO, F_SETFL, oldf);

//     if (ch != EOF) {
//         ungetc(ch, stdin);
//         return 1;
//     }

//     return 0;
// }

// char getch(void) {
//     struct termios oldt, newt;
//     char ch;
//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//     ch = getchar();
//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     return ch;
// }

// // Função principal do jogo
// void jogar() {
//     inicializar();
//     while (1) {
//         desenhar();
//         atualizar();
//         if (kbhit()) {
//             char tecla = getch();
//             if (tecla == ' ') nave_y += PULO;  // Pular ao pressionar espaço
//         }
//         usleep(100000);  // Controla a velocidade do jogo (100 milissegundos)
//     }
// }

// int main() {
//     printf("Bem-vindo ao Jogo da Nave e Espinhos!\n");
//     printf("Pressione ESPAÇO para fazer a nave subir.\n");
//     printf("Pressione qualquer tecla para começar...\n");
//     getch();
//     jogar();
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>

#define GRAVIDADE 1
#define PULO -3
#define ESPINHOS_FREQUENCIA 5

// Estrutura para representar a nave
typedef struct {
    int x;
    int y;
} Nave;

// Estrutura para representar um espinho
typedef struct {
    int x;
    int y;
} Espinho;

// Estrutura para representar o estado do jogo
typedef struct {
    Nave nave;
    Espinho* espinhos;
    int pontuacao;
    int largura;
    int altura;
    int num_espinhos;
} Jogo;

// Função para inicializar o jogo
void inicializar(Jogo* jogo) {
    srand(time(0));

    // Inicializar nave
    jogo->nave.x = 5;
    jogo->nave.y = jogo->altura / 2;

    // Inicializar espinhos
    jogo->espinhos = (Espinho*)malloc(jogo->num_espinhos * sizeof(Espinho));
    for (int i = 0; i < jogo->num_espinhos; i++) {
        jogo->espinhos[i].x = jogo->largura + i * ESPINHOS_FREQUENCIA;
        jogo->espinhos[i].y = rand() % jogo->altura;
    }

    jogo->pontuacao = 0;
}

// Função para desenhar o jogo
void desenhar(Jogo* jogo) {
    system("clear");

    for (int y = 0; y < jogo->altura; y++) {
        for (int x = 0; x < jogo->largura; x++) {
            if (x == jogo->nave.x && y == jogo->nave.y) {
                printf("A");  // A nave
            } else {
                int ehEspinho = 0;
                for (int i = 0; i < jogo->num_espinhos; i++) {
                    if (x == jogo->espinhos[i].x && y == jogo->espinhos[i].y) {
                        printf("^");  // Espinho
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

// Função para atualizar o estado do jogo
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
            free(jogo->espinhos); // Libera memória dos espinhos
            free(jogo); // Libera memória do jogo
            exit(0);
        }
    }

    if (jogo->nave.y < 0 || jogo->nave.y >= jogo->altura) {
        printf("Game Over! Sua pontuação foi: %d\n", jogo->pontuacao);
        free(jogo->espinhos); // Libera memória dos espinhos
        free(jogo); // Libera memória do jogo
        exit(0);
    }
}

// Funções para simular _kbhit() e _getch() no macOS/Linux
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

// Função principal do jogo
void jogar(Jogo* jogo) {
    inicializar(jogo);
    while (1) {
        desenhar(jogo);
        atualizar(jogo);
        if (kbhit()) {
            char tecla = getch();
            if (tecla == ' ') jogo->nave.y += PULO;  // Pular ao pressionar espaço
        }
        usleep(100000);  // Controla a velocidade do jogo (100 milissegundos)
    }
}

int main() {
    // Alocar memória para a estrutura Jogo
    Jogo* jogo = (Jogo*)malloc(sizeof(Jogo));
    if (!jogo) {
        fprintf(stderr, "Erro ao alocar memória para o jogo.\n");
        return 1;
    }

    // Inicializar parâmetros do jogo
    jogo->largura = 30;
    jogo->altura = 20;
    jogo->num_espinhos = 30;

    printf("Bem-vindo ao Jogo da Nave e Espinhos!\n");
    printf("Pressione ESPAÇO para fazer a nave subir.\n");
    printf("Pressione qualquer tecla para começar...\n");
    getch();
    jogar(jogo);

    // Liberar memória alocada para o jogo ao final
    free(jogo->espinhos);
    free(jogo);

    return 0;
}
