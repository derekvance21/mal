#include <stdlib.h>
#include <string.h>
#include "eval.h"
#include "types.h"


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
    // could use new env_init if let* was used like (let* ((x y z) (1 2 3)) (+ x y z)),
    // but then you won't get the left to right evaluated binding
    env_t let_env = env_init(env, NULL, NULL, 0);
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

// ast is a list where first element is symbol 'if
mal_t eval_if(mal_t ast, env_t *env)
{
    if (ast.val.list->len != 4)
    {
        return mal_error("Syntax Error: special form 'if requires three parameters");
    }

    mal_t condition = eval_ast(((mal_t*)ast.val.list->items)[1], env);
    if (condition.type == KEYWORD && (condition.val.keyword == FALSE || condition.val.keyword == NIL))
    {
        return eval_ast(((mal_t*)ast.val.list->items)[3], env);
    }
    else
    {
        return eval_ast(((mal_t*)ast.val.list->items)[2], env);
    }
}

// ast is a list where first element is symbol 'lambda
mal_t eval_lambda(mal_t ast, env_t *env)
{
    mal_t bindings, body;

    if (ast.val.list->len != 3)
    {
        return mal_error("Syntax Error: special form 'lambda requires two parameters");
    }

    bindings = ((mal_t*)ast.val.list->items)[1];
    if (bindings.type != LIST)
    {
        return mal_error("Syntax Error: special form 'lambda requires list of bindings as first parameter");
    }

    body = ((mal_t*) ast.val.list->items)[2];

    return mal_closure(bindings.val.list->len, (mal_t*)bindings.val.list->items, body, env);
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

        if (i == 0 && element.type != FUNCTION && element.type != CLOSURE)
        {
            // with static vector_t vector_init(...) function, this free is unnecessary
            vector_free(elements);
            return mal_error("Error: Cannot apply non-function mal_t type");
        }

        int pos = vector_push(elements);
        ((mal_t*)elements->items)[pos] = element;
    }

    mal_t func = ((mal_t*)elements->items)[0];
    mal_t result;
    if (func.type == CLOSURE)
    {
        // Third parameter is type mal_t* but env_init needs vector_t*
        // TODO: check that closure argc == ast.val.list->len - 1
        if (ast.val.list->len - 1 != func.val.closure->argc)
        {
            result = mal_error("Error: parity mismatch");
        }
        else
        {
            env_t *closure_env = (env_t*) malloc(sizeof(env_t));
            *closure_env = env_init(env, func.val.closure->params, &((mal_t*)ast.val.list->items)[1], func.val.closure->argc);
            result = eval_ast(func.val.closure->body, closure_env);
        }
    }
    else
    {
        // apply func, with argument list length and list of argument mal_t elements
        result = func.val.func(elements->len - 1, &((mal_t*) elements->items)[1]);
    }
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
        else if (strcmp(first.val.symbol, "if") == 0)
        {
            return eval_if(ast, env);
        }
        else if (strcmp(first.val.symbol, "lambda") == 0)
        {
            return eval_lambda(ast, env);
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