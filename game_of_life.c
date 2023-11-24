#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 25
#define COLS 80

void print_board(int board[ROWS][COLS]);
void update_board(int board[ROWS][COLS]);
int neighbors(int i, int j, int board[ROWS][COLS]);
void update_cell(int new_board[ROWS][COLS], int board[ROWS][COLS]);
void read_file(int matrix[ROWS][COLS]);
void assignment(int matrix[ROWS][COLS], int board[ROWS][COLS]);
int checking_board(int board[ROWS][COLS]);
// void end_of_game();
void pressed_key();

// int board[ROWS][COLS];

int main() {
    int board[ROWS][COLS];
    int condition[ROWS][COLS];
    read_file(condition);
    initscr();
    assignment(condition, board);
    do {
        refresh();
        clear();
        print_board(board);
        pressed_key();
        update_board(board);
        // usleep(100000);

    } while (checking_board(board) != 1);
    //   end_of_game();
    // endwin();

    return 0;
}
void read_file(int matrix[ROWS][COLS]) {  //чтениеи файла
    int nfile;
    FILE *file = 0;
    // clear();
    initscr();
    printw("Enter the initial configuration number(1-5):\n1. Ship");

    if (scanf("%d", &nfile) == 1) {
        // getch();
        switch (nfile) {
            case 1:
                file = fopen("1.txt", "r");
                break;
            case 2:
                file = fopen("2.txt", "r");
                break;
            case 3:
                file = fopen("3.txt", "r");
                break;
            case 4:
                file = fopen("4.txt", "r");
                break;
            case 5:
                file = fopen("5.txt", "r");
                break;
            default:
                printf("Enter the number from 1 to 5");
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);
}
int checking_board(int board[ROWS][COLS]) {  // проверка поля, если поколение
                                             // повторяется игра заканчивается
    int c[ROWS][COLS];
    int flag = 0, k = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            c[i][j] = board[i][j];
        }
    }
    update_board(board);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if ((c[i][j] == board[i][j]) || (board[i][j] == 0)) {
                k++;
            }
            // else k = 0;
        }
    }
    if (k == 2000) {
        flag = 1;
    } else {
        flag = 0;
    }
    return flag;
}
void print_board(int board[ROWS][COLS]) {  // Отрисовка поля
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 0)
                printw(".");
            else
                printw("o");
        }
        printw("\n");
    }
}
void assignment(int matrix[ROWS][COLS], int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = matrix[i][j];
        }
    }
}
int neighbors(int i, int j, int board[ROWS][COLS]) {
    int live_neighbors = 0;
    for (int k = -1; k <= 1; k++) {  //подсчет количества живых соседей
        for (int l = -1; l <= 1; l++) {
            if (k == 0 && l == 0) continue;  //пропускаем текущую клетку

            int x = i + k;
            int y = j + l;

            if (x < 0)  //выход за пределы поля
                x = ROWS - 1;
            else if (x >= ROWS)
                x = 0;
            if (y < 0)
                y = COLS - 1;
            else if (y >= COLS)
                y = 0;

            if (x >= 0 && x < ROWS && y >= 0 &&
                y < COLS)  //проверяем, находится ли соседняя клетка внутри поля
                live_neighbors += board[x][y];
        }
    }
    return live_neighbors;
}
void update_cell(int new_board[ROWS][COLS], int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            //обновление состояния клетки
            if (board[i][j] == 1 &&
                (neighbors(i, j, board) < 2 || neighbors(i, j, board) > 3))  //клетка умирает
                new_board[i][j] = 0;
            else if (board[i][j] == 0 && neighbors(i, j, board) == 3)
                new_board[i][j] = 1;  //в мертвой клетке рождается новая клетка
            else
                new_board[i][j] = board[i][j];
        }
    }
}
void update_board(int board[ROWS][COLS]) {  //обновление поля
    int new_board[ROWS][COLS];
    update_cell(new_board, board);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = new_board[i][j];
        }
    }
}

void pressed_key() {
    int delay = 70000;
    nodelay(stdscr, true);
    char ch = getch();
    noecho();
    if (ch) {  // если нажата клавиша
        printw("%c", ch);

        if (ch == '=') {  // увеличить скорость
            delay -= 100000;
        } else if (ch == '-') {  // уменьшить скорость
            delay += 100000;
        }
        if (delay <= 10000) delay = 10000;
    }
    usleep(delay);  // задержка
}

// void end_of_game() {
//   printw("   EEEEEE  N     N  DDDDD         OOOOO    FFFFFF       GGGGGGG     "
//          "AAA    MM   MM  EEEEEE\n");
//   printw("   E       N N   N  D    D       O     O   F           G           A "
//          "  A   M M M M  E     \n");
//   printw("   EEEEEE  N  N  N  D     D     O       O  FFFFFF      G  GGGGG   "
//          "AAAAAAA  M  M  M  EEEE  \n");
//   printw("   E       N   N N  D    D       O     O   F           G      G   A  "
//          "   A  M     M  E     \n");
//   printw("   EEEEEE  N     N  DDDDD         OOOOO    F            GGGGGG    A  "
//          "   A  M     M  EEEEEE\n");
// }
