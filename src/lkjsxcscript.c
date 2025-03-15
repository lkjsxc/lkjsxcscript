#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

struct termios term_old;
struct termios term_new;

void term_init() {
    tcgetattr(STDIN_FILENO, &term_old);
    term_new = term_old;
    term_new.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    term_new.c_oflag &= ~(OPOST);
    term_new.c_cflag |= (CS8);
    term_new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    term_new.c_cc[VMIN] = 0;
    term_new.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_new);
}

int main() {
    term_init();
    while (1) {
        char c;
        if (read(0, &c, 1) == 1) {
            if (c == 0x1b) {
                break;
            }
            printf("%c", c);
        }
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_old);
    return 0;
}