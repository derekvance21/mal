#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "env.h"
#include "types.h"

const int BUCKETS = 16;


unsigned int hash(char *key)
{
    unsigned int hash = 5381;
    char c;

    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

// binds is vector_t<mal_t>, exprs is vector_t<mal_t>; they should be the same len
env_t env_init(env_t *outer, mal_t *binds, mal_t *exprs, int argc)
{
    env_t env;
    env.outer = outer;
    int i;
    for (i = 0; i < BUCKETS; ++i)
    {
        env.buckets[i] = NULL;
    }

    // do bindings
    if (binds && exprs)
    {
        for (i = 0; i < argc; ++i)
        {
            mal_t bind = binds[i];
            mal_t expr = exprs[i];
            env_insert(&env, bind.val.symbol, expr);
        }
    }

    return env;
}

int env_insert(env_t *env, char *key, mal_t mal)
{
    unsigned int bucket = hash(key) % BUCKETS;

    // first in bucket
    if (!env->buckets[bucket])
    {
        env->buckets[bucket] = malloc(sizeof(env_element_t));
        if (!env->buckets[bucket])
        {
            perror("error allocating memory for env");
            exit(1);
        }
        env_element_t insert;
        insert.key = key;
        insert.mal = mal;
        insert.next = NULL;
        *env->buckets[bucket] = insert;
        return 0;
    }

    env_element_t *curr = env->buckets[bucket];
    while (curr)
    {
        if (strcmp(key, curr->key) == 0)
        {
            curr->mal = mal;
            return 0;
        }
        if (!curr->next) {
            break; 
        }
        curr = curr->next;
    }

    curr->next = malloc(sizeof(env_element_t));
    if (!curr->next)
    {
        perror("error allocating memory for env");
        exit(1);
    }
    env_element_t new;
    new.key = key;
    new.mal = mal;
    new.next = NULL;
    *curr->next = new;
    return 0;
}

mal_t env_get(env_t *env, char *key)
{
    unsigned int bucket = hash(key) % BUCKETS;

    env_t *curr_env = env;
    while (curr_env)
    {
        env_element_t *curr_elm = curr_env->buckets[bucket];
        while (curr_elm)
        {
            if (strcmp(key, curr_elm->key) == 0)
            {
                return curr_elm->mal;
            }
            curr_elm = curr_elm->next;
        }

        // couldn't find key in current env, moving to outer
        curr_env = curr_env->outer;
    }

    return mal_error("Runtime Error: cannot reference an identifier before its definition");
}

void env_free(env_t *env)
{
    int i;
    for (i = 0; i < BUCKETS; ++i)
    {
        env_element_t *next;
        env_element_t *curr = env->buckets[i];
        while (curr)
        {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }
}
