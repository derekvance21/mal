#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "types.h"
#include "reader.h"
#include "printer.h"
#include "env.h"

mal_t EVAL(mal_t ast, env_t *env);

mal_t eval_ast(mal_t ast, env_t *env)
{
    switch (ast.type)
    {
        case SYMBOL:
            return env_get(env, ast.val.symbol);
        case LIST:
            {
                int i;
                vector_t *elements = vector_init(sizeof(mal_t));
                for (i = 0; i < ast.val.list->len; ++i)
                {
                    int pos = vector_push(elements);
                    ((mal_t*)elements->items)[pos] = EVAL(((mal_t*)ast.val.list->items)[i], env);
                }
                return mal_list(elements);
            }
        default:
            return ast;
        
    }
}

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

char* rep(char *in, env_t *env)
{
    return PRINT(EVAL(READ(in), env));
}

int main(int argc, char *argv[])
{
    env_t repl_env = env_init();
    char *env_str = "env!";
    char *env_test = malloc(strlen(env_str));
    strcpy(env_test, env_str);
    // TODO: this is all sorts of freeing errors
    env_insert(&repl_env, "+", mal_error(env_test));
    env_insert(&repl_env, "-", mal_error(env_test));
    env_insert(&repl_env, "*", mal_error(env_test));
    env_insert(&repl_env, "/", mal_error(env_test));

    char *line;
    for (;;) {
        line = readline("> ");
        if (!line) {
            printf("\n");
            break;
        }
        if (*line) {
            add_history(line);
            printf("%s\n", rep(line, &repl_env));
        }
        free(line);
    }
    env_free(&repl_env);
}