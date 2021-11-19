#ifndef ENV_H
#define ENV_H

#include "types.h"

typedef struct env_element
{
    char *key;
    mal_t mal;
    struct env_element *next;
} env_element_t;

typedef struct env
{
    env_element_t *buckets[16];
} env_t;

env_t env_init();
int env_insert(env_t *env, char *key, mal_t mal);
mal_t env_get(env_t *env, char *key);
void env_free(env_t *env);


#endif