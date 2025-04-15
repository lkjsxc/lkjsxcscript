#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MEMORY_SIZE (1024 * 16)
#define PORT 8080
#define ISDEBUG 1

enum result {
    RESULT_OK,
    RESULT_ERR,
};
enum bool {
    FALSE,
    TRUE,
};

enum type {
    INST_NULL,
    INST_NOP,
    INST_PUSH_CONST,
    INST_PUSH_VAR,
    INST_GLOBAL_GET,
    INST_GLOBAL_SET,
    INST_CALL,
    INST_RETURN,
    INST_JUMP,
    INST_JZE,
    INST_OR,
    INST_AND,
    INST_NOT,
    INST_EQ,
    INST_NE,
    INST_LT,
    INST_GT,
    INST_ADD,
    INST_SUB,
    INST_MUL,
    INST_DIV,
    INST_MOD,
    INST_READ,
    INST_WRITE,
    LABEL_OPEN,
    LABEL_CLOSE,
    INST_ACCEPT,
    INST_USLEEP,
};

struct vec {
    char* data;
    int size;
};

struct node {
    struct vec* token;
    struct node* next;
    struct node* lhs;
    struct node* rhs;
    int64_t value;
    enum type type;
};

struct compiler {
    char code[MEMORY_SIZE / 32];
    char src[MEMORY_SIZE / 32];
    struct vec token[MEMORY_SIZE / 32];
    struct node node[MEMORY_SIZE / 32];
    struct node map_key[MEMORY_SIZE / 32];
    struct node map_value[MEMORY_SIZE / 32];
    int node_size;
    int map_size;
};

union mem {
    int64_t i64[MEMORY_SIZE / sizeof(int64_t)];
    struct compiler compiler;
};

static union mem mem;

enum result compile_readsrc(char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", path);
        return RESULT_ERR;
    }
    size_t bytes_read = fread(mem.compiler.src, 1, sizeof(mem.compiler.src), file);
    fclose(file);
    mem.compiler.src[bytes_read] = '\n';
    mem.compiler.src[bytes_read + 1] = '\0';
    return RESULT_OK;
}

enum result compile_tokenize() {
    struct vec* token_end = mem.compiler.token;
    char* base = mem.compiler.src;
    char* itr = base;
    for (; *itr != '\0'; itr++) {
        if (*itr == ' ' || *itr == '\n' || *itr == '\t') {
            if (base != itr) {
                *(token_end++) = (struct vec){.data = base, .size = itr - base};
            }
            base = itr + 1;
            continue;
        }
        if ((*itr == '=' && *(itr + 1) == '=') || (*itr == '!' && *(itr + 1) == '=') ||
            (*itr == '<' && *(itr + 1) == '=') || (*itr == '>' && *(itr + 1) == '=') ||
            (*itr == '&' && *(itr + 1) == '&') || (*itr == '|' && *(itr + 1) == '|')) {
            if (base != itr) {
                *(token_end++) = (struct vec){.data = base, .size = itr - base};
            }
            *(token_end++) = (struct vec){.data = itr, .size = 2};
            base = itr + 2;
            itr += 1;
            continue;
        }
        if (*itr == '+' || *itr == '-' || *itr == '*' || *itr == '/' || *itr == '%' ||
            *itr == '=' || *itr == '!' || *itr == '<' || *itr == '>' || *itr == '&' ||
            *itr == '|' || *itr == '^' || *itr == '~' || *itr == '(' || *itr == ')' ||
            *itr == '{' || *itr == '}' || *itr == '[' || *itr == ']' || *itr == ';' ||
            *itr == ',' || *itr == ':' || *itr == '.' || *itr == '?' || *itr == '!' ||
            *itr == '\'' || *itr == '\"' || *itr == '`' || *itr == '\\') {
            if (base != itr) {
                *(token_end++) = (struct vec){.data = base, .size = itr - base};
            }
            *(token_end++) = (struct vec){.data = itr, .size = 1};
            base = itr + 1;
            continue;
        }
    }
    *token_end = (struct vec){.data = NULL, .size = 0};
}

enum result compile_pushnode(struct node* dst, struct vec* token) {
    if (mem.compiler.node_size >= sizeof(mem.compiler.node) / sizeof(struct node)) {
        fprintf(stderr, "Error: node size exceeded\n");
        return RESULT_ERR;
    }
    struct node* node = &mem.compiler.node[mem.compiler.node_size++];
    node->token = token;
    node->next = dst;
    node->lhs = NULL;
    node->rhs = NULL;
    dst = node;
    return RESULT_OK;
}

enum bool token_eq(struct vec* token, struct vec* str) {
    if (token->size != str->size) {
        return FALSE;
    }
    return memcmp(token->data, str->data, token->size) == 0;
}

enum bool token_eq_str(struct vec* token, char* str) {
    size_t len = strlen(str);
    if (token->size != len) {
        return FALSE;
    }
    return memcmp(token->data, str, len) == 0;
}

enum result token_to_int(struct vec* token, int64_t* dst) {
    int64_t value = 0;
    for(size_t i = 0; i < token->size; i++) {
        if (token->data[i] < '0' || token->data[i] > '9') {
            return RESULT_ERR;
        }
        value = value * 10 + (token->data[i] - '0');
    }
    *dst = value;
    return RESULT_OK;
}

enum result compile_parse() {
}

enum result compile_analyze() {
}

enum result compile_optimize() {
}

enum result compile_generate() {
}

enum result compile(char* path) {
    if (compile_readsrc(path) == RESULT_ERR) {
        fprintf(stderr, "Error reading source file: %s\n", path);
        return RESULT_ERR;
    }
    if (compile_tokenize() == RESULT_ERR) {
        fprintf(stderr, "Error tokenizing source file: %s\n", path);
        return RESULT_ERR;
    }
    if (compile_parse() == RESULT_ERR) {
        fprintf(stderr, "Error parsing source file: %s\n", path);
        return RESULT_ERR;
    }
    if (compile_analyze() == RESULT_ERR) {
        fprintf(stderr, "Error analyzing source file: %s\n", path);
        return RESULT_ERR;
    }
    if (compile_optimize() == RESULT_ERR) {
        fprintf(stderr, "Error optimizing source file: %s\n", path);
        return RESULT_ERR;
    }
    if (compile_generate() == RESULT_ERR) {
        fprintf(stderr, "Error generating code for source file: %s\n", path);
        return RESULT_ERR;
    }
    return RESULT_OK;
}

enum result execute() {
}

int main(int argc, char** argv) {
#if ISDEBUG
    if (compile("./lkjsxcscript_src") == RESULT_ERR) {
        fprintf(stderr, "Compilation failed\n");
        return 1;
    }
#else
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }
    if (compile(argv[1]) == RESULT_ERR) {
        fprintf(stderr, "Compilation failed\n");
        return 1;
    }
#endif
    if (execute() == RESULT_ERR) {
        fprintf(stderr, "Execution failed\n");
        return 1;
    }
    return 0;
}