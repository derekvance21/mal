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
        {
            mal_t mal = env_get(env, ast.val.symbol);
            // ast.val.symbol was a unique malloc'd token, so free it after indexing into env
            free(ast.val.symbol);
            return mal;
        }
        case LIST:
            {
                int i;
                vector_t *elements = vector_init(sizeof(mal_t));
                for (i = 0; i < ast.val.list->len; ++i)
                {
                    int pos = vector_push(elements);
                    ((mal_t*)elements->items)[pos] = EVAL(((mal_t*)ast.val.list->items)[i], env);
                }
                // apply the first element's procedure to the arguments
                // then free(elements)
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
    env_insert(&repl_env, "+", mal_integer(42));
    env_insert(&repl_env, "-", mal_integer(42));
    env_insert(&repl_env, "*", mal_integer(42));
    env_insert(&repl_env, "/", mal_integer(42));

    char *line;
    for (;;) {
        line = readline("> ");
        if (!line) {
            printf("\n");
            break;
        }
        if (*line) {
            add_history(line);
            char *rep_str = rep(line, &repl_env);
            printf("%s\n", rep_str);
            // rep_str should be free'd, except for errors
        }
        free(line);
    }
    env_free(&repl_env);
}