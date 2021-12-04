#ifndef EVAL_H
#define EVAL_H

#include "types.h"
#include "env.h"

mal_t eval_def(mal_t ast, env_t *env);
mal_t eval_let(mal_t ast, env_t *env);
mal_t eval_app(mal_t ast, env_t *env);
mal_t eval_list(mal_t ast, env_t *env);
mal_t eval_ast(mal_t ast, env_t *env);

#endif