#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

char* READ(char *in) {
    return in;
}

char* EVAL(char *in) {
    return in;
}

char* PRINT(char *in) {
    return in;
}

char* rep(char *in) {
    return PRINT(EVAL(READ(in)));
}

int main(int argc, char *argv[]) {
    char *line;
    for (;;) {
        line = readline("user> ");
        if (!line) {
            printf("\n");
            break;
        }
        if (*line) {
            add_history(line);
            printf("%s\n", rep(line));
        }
        free(line);
    }
}