#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 20
#define PADDLE_HEIGHT 3

typedef struct {
    int x, y;
    int dy; // direção do paddle
} Paddle;

typedef struct {
    int x, y;
    int dx, dy; // direção da bola
} Ball;

void initialize();
void draw(Paddle *p1, Paddle *p2, Ball *ball);
void move_ball(Ball *ball, Paddle *p1, Paddle *p2);
void move_paddle(Paddle *paddle, int direction);

int main() {
    Paddle player1 = {1, HEIGHT / 2 - 1, 0};
    Paddle player2 = {WIDTH - 2, HEIGHT / 2 - 1, 0};
    Ball ball = {WIDTH / 2, HEIGHT / 2, 1, 1};

    initialize();

    while (1) {
        clear();
        draw(&player1, &player2, &ball);

        // Move the ball
        move_ball(&ball, &player1, &player2);

        // Get user input
        int ch = getch();
        if (ch == 'w') move_paddle(&player1, -1);
        if (ch == 's') move_paddle(&player1, 1);
        if (ch == 'i') move_paddle(&player2, -1);
        if (ch == 'k') move_paddle(&player2, 1);
        if (ch == 'q') break; // sair do jogo

        usleep(100000); // controla a velocidade do jogo
    }

    endwin();
    return 0;
}

void initialize() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // não bloquear na entrada
    srand(time(NULL));
}

void draw(Paddle *p1, Paddle *p2, Ball *ball) {
    // Desenha paddles
    for (int i = 0; i < PADDLE_HEIGHT; i++) {
        mvprintw(p1->y + i, p1->x, "#");
        mvprintw(p2->y + i, p2->x, "#");
    }
    
    // Desenha a bola
    mvprintw(ball->y, ball->x, "O");
    
    // Atualiza a tela
    refresh();
}

void move_ball(Ball *ball, Paddle *p1, Paddle *p2) {
    ball->x += ball->dx;
    ball->y += ball->dy;

    // Colisão com as bordas
    if (ball->y <= 0 || ball->y >= HEIGHT - 1) {
        ball->dy *= -1; // inverte a direção vertical
    }

    // Colisão com paddles
    if ((ball->x == p1->x + 1 && ball->y >= p1->y && ball->y < p1->y + PADDLE_HEIGHT) ||
        (ball->x == p2->x - 1 && ball->y >= p2->y && ball->y < p2->y + PADDLE_HEIGHT)) {
        ball->dx *= -1; // inverte a direção horizontal
    }

    // Reinicia a bola se sair pela lateral
    if (ball->x <= 0 || ball->x >= WIDTH - 1) {
        ball->x = WIDTH / 2;
        ball->y = HEIGHT / 2;
        ball->dx = (rand() % 2 == 0) ? 1 : -1;
        ball->dy = (rand() % 2 == 0) ? 1 : -1;
    }
}

void move_paddle(Paddle *paddle, int direction) {
    paddle->y += direction;
    // Restringe movimento dos paddles
    if (paddle->y < 0) paddle->y = 0;
    if (paddle->y > HEIGHT - PADDLE_HEIGHT) paddle->y = HEIGHT - PADDLE_HEIGHT;
}