#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static struct termios term_orig;

void term_deinit() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_orig);
}

void term_init() {
    static struct termios term_new;
    tcgetattr(STDIN_FILENO, &term_orig);
    term_new = term_orig;
    term_new.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    term_new.c_oflag &= ~(OPOST);
    term_new.c_cflag |= (CS8);
    term_new.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    term_new.c_cc[VMIN] = 0;
    term_new.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term_new);
}

void compoile_deinit() {
}

void compile_init() {
}

void compile_tokenize() {
}

void compile_parse() {
}

void compile_analyze() {
}

void compile_optimize() {
}

void compile_generate() {
}

void compile() {
}

int main() {
    term_init();
    while (1) {
        char c;
        if (read(0, &c, 1) == 0) {
            continue;
        }
        if (c == 0x1b) {
            break;
        }
        write(STDOUT_FILENO, &c, 1);
    }
    term_deinit();
    return 0;
}