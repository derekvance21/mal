#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "types.h"
#include "reader.h"
#include "printer.h"
#include "env.h"
#include "core.h"
#include "eval.h"


mal_t READ(char *in)
{
    return read_str(in);
}

mal_t EVAL(mal_t ast, env_t *env)
{
    return eval_ast(ast, env);
}

char* PRINT(mal_t ast)
{
    return pr_str(ast);
}

char* rep(char *in, env_t *env, int *err)
{
    mal_t ast = EVAL(READ(in), env);
    // TODO: Make this way smarter
    if (ast.type == ERROR || ast.type == KEYWORD || ast.type == FUNCTION || ast.type == CLOSURE)
    {
        *err = 1;
    }
    return PRINT(ast);
}

int main(int argc, char *argv[])
{
    env_t repl_env = env_init(NULL, NULL, NULL, 0);
    env_insert(&repl_env, "+", mal_func(add));
    env_insert(&repl_env, "-", mal_func(sub));
    env_insert(&repl_env, "*", mal_func(mul));
    env_insert(&repl_env, "/", mal_func(divide));
    env_insert(&repl_env, "=", mal_func(eq));
    env_insert(&repl_env, "list", mal_func(list));
    env_insert(&repl_env, "list?", mal_func(is_list));
    env_insert(&repl_env, "empty?", mal_func(is_empty));
    env_insert(&repl_env, "length", mal_func(length));

    char *line;
    for (;;) {
        line = readline("> ");
        if (!line) {
            printf("\n");
            break;
        }
        if (*line) {
            add_history(line);
            int err = 0;
            char *rep_str = rep(line, &repl_env, &err);
            printf("%s\n", rep_str);

            // as of now, errors print as string literals
            // so all non-errors have to free rep_str
            if (err == 0)
            {
                free(rep_str);
            }
        }
        free(line);
    }
    env_free(&repl_env);
}