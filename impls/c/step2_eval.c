// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include "types.h"
// #include "reader.h"
// #include "printer.h"
// #include "env.h"
// #include "core.h"
// #include "eval.h"


// mal_t READ(char *in)
// {
//     return read_str(in);
// }

// mal_t EVAL(mal_t ast, env_t *env)
// {
//     return eval_ast(ast, env);
// }

// char* PRINT(mal_t ast)
// {
//     return pr_str(ast);
// }

// char* rep(char *in, env_t *env)
// {
//     return PRINT(EVAL(READ(in), env));
// }

// int main(int argc, char *argv[])
// {
//     env_t repl_env = env_init();
//     env_insert(&repl_env, "+", mal_func(add));
//     env_insert(&repl_env, "-", mal_func(sub));
//     env_insert(&repl_env, "*", mal_func(mul));
//     env_insert(&repl_env, "/", mal_func(divide));

//     char *line;
//     for (;;) {
//         line = readline("> ");
//         if (!line) {
//             printf("\n");
//             break;
//         }
//         if (*line) {
//             add_history(line);
//             char *rep_str = rep(line, &repl_env);
//             printf("%s\n", rep_str);
//             // rep_str should be free'd, except for errors
//         }
//         free(line);
//     }
//     env_free(&repl_env);
// }