#include <stdlib.h>
#include <string.h>
#include "eval.h"


// ast is a list with the first element being the symbol 'def!
mal_t eval_def(mal_t ast, env_t *env)
{
    // ast needs to be a list of length 3, or else parity mismatch
    // TODO: Don't require list of length 3. The body can be any number of expressions, where only the last is returned
    // first parameter is unevaluated symbol
    // second parameter is evaluated mal_t, where cannot be error
    if (ast.val.list->len != 3)
    {
        return mal_error("Syntax Error: special form 'def! requires two parameters");
    }

    mal_t id = ((mal_t*)ast.val.list->items)[1];
    if (id.type != SYMBOL)
    {
        return mal_error("Syntax Error: special form 'def! requires first parameter to be symbol");
    }

    mal_t val = eval_ast(((mal_t*)ast.val.list->items)[2], env);
    if (val.type == ERROR)
    {
        return val;
    }

    env_insert(env, id.val.symbol, val);
    return val;
}

// ast is a list with the first element being the symbol 'let*
// ex: (let* ((x 2) (y (+ x 1))) y) == 3
// notice the bindings to the left are visible to those on the right
mal_t eval_let(mal_t ast, env_t *env)
{
    // ast needs to be list of length 3, or else parity mismatch
    // TODO: Don't require list of length 3. The body can be any number of expressions, where only the last is returned
    // first parameter is list, where each element is list of length 2 (bindings)
    // second parameter is evaluated mal_t with new env created from first parameter bindings
    if (ast.val.list->len != 3)
    {
        return mal_error("Syntax Error: special form 'let* requires two parameters");
    }
    
    mal_t bindings = ((mal_t*)ast.val.list->items)[1];
    if (bindings.type != LIST)
    {
        return mal_error("Syntax Error: special form 'let* requires first parameter to be list");
    }

    // create a new env for the let expression, to be added to with the bindings
    env_t let_env = env_init(env);
    int i;
    for (i = 0; i < bindings.val.list->len; ++i)
    {
        mal_t binding = ((mal_t*)bindings.val.list->items)[i];
        if (binding.type != LIST || binding.val.list->len != 2)
        {
            env_free(&let_env);
            return mal_error("Syntax Error: special form 'let* requires each element of bindings to be list of length 2");
        }

        mal_t id = ((mal_t*)binding.val.list->items)[0];
        if (id.type != SYMBOL)
        {
            env_free(&let_env);
            return mal_error("Syntax Error: special form 'let* requires first element of each binding to be symbol");
        }

        mal_t val = eval_ast(((mal_t*)binding.val.list->items)[1], &let_env);
        if (val.type == ERROR)
        {
            env_free(&let_env);
            return val;
        }

        env_insert(&let_env, id.val.symbol, val);
    }

    // evaluate the body with the let_env
    mal_t body = ((mal_t*)ast.val.list->items)[2];
    mal_t result = eval_ast(body, &let_env);
    env_free(&let_env);
    return result;
}

// ast is a list with at least one element which is not a special form symbol
mal_t eval_app(mal_t ast, env_t *env)
{
    int i;
    vector_t *elements = vector_init(sizeof(mal_t));
    for (i = 0; i < ast.val.list->len; ++i)
    {
        // IMPORTANT: This used to be EVAL (from step2_eval.c), and that's what the guide says, but I don't see a reason for that, so it recurses with eval_ast
        mal_t element = eval_ast(((mal_t*)ast.val.list->items)[i], env);
        if (element.type == ERROR)
        {
            vector_free(elements);
            return element;
        }

        if (i == 0 && element.type != FUNCTION)
        {
            // with static vector_t vector_init(...) function, this free is unnecessary
            vector_free(elements);
            return mal_error("Error: Cannot apply non-function mal_t type");
        }

        int pos = vector_push(elements);
        ((mal_t*)elements->items)[pos] = element;
    }

    mal_t func = ((mal_t*)elements->items)[0];
    // apply func, with argument list length and list of argument mal_t elements
    mal_t result = func.val.func(elements->len - 1, &((mal_t*) elements->items)[1]);
    vector_free(elements);
    return result;
}

// ast is a list
mal_t eval_list(mal_t ast, env_t *env)
{
    if (ast.val.list->len == 0)
    {
        return mal_error("Error: Cannot evaluate empty list");
    }

    // check if special form
    mal_t first = ((mal_t*)ast.val.list->items)[0];
    if (first.type == SYMBOL)
    {
        if (strcmp(first.val.symbol, "def!") == 0)
        {
            return eval_def(ast, env);
        }
        else if (strcmp(first.val.symbol, "let*") == 0)
        {
            return eval_let(ast, env);
        }
    }

    // it's not a special form, so apply normal application rules
    return eval_app(ast, env);
}

// ast is any mal_t
mal_t eval_ast(mal_t ast, env_t *env)
{
    switch (ast.type)
    {
        case SYMBOL:
        {
            mal_t mal = env_get(env, ast.val.symbol);
            // ast.val.symbol was a unique malloc'd token, so free it after indexing into env
            // TODO: Replace this with mal_free(ast)
            free(ast.val.symbol);
            return mal;
        }
        case LIST:
            return eval_list(ast, env);
        default:
            return ast;
        
    }
}