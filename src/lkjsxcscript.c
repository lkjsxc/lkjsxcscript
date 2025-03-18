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

void vm_compile_tokenize() {
}

void vm_compile_parse() {
}

void vm_compile_analyze() {
}

void vm_compile_optimize() {
}

void vm_compile_generate() {
}

void vm_compile() {
}

void vm_execute() {
}

void vm_deinit() {
}

void vm_init() {
}

int main() {
    term_init();
    vm_init();
    vm_compile();
    vm_execute();
    vm_deinit();
    term_deinit();
    return 0;
}