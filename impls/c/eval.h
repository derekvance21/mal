#ifndef EVAL_H
#define EVAL_H

#include "types.h"
#include "env.h"

mal_t eval_ast(mal_t ast, env_t *env);

#endif