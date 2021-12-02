#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "env.h"
#include "types.h"

const int BUCKETS = 16;


int hash(char *key)
{
    int hash = 5381;
    char c;

    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

env_t env_init()
{
    env_t env;
    int i;
    for (i = 0; i < BUCKETS; ++i)
    {
        env.buckets[i] = NULL;
    }
    return env;
}

int env_insert(env_t *env, char *key, mal_t mal)
{
    int bucket = hash(key) % BUCKETS;

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
    env_element_t insert;
    insert.key = key;
    insert.mal = mal;
    insert.next = NULL;
    *curr->next = insert;
    return 0;
}

mal_t env_get(env_t *env, char *key)
{
    int bucket = hash(key) % BUCKETS;
    env_element_t *curr = env->buckets[bucket];
    while (curr)
    {
        if (strcmp(key, curr->key) == 0)
        {
            return curr->mal;
        }
        curr = curr->next;
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
