#ifndef CORE_H
#define CORE_H


mal_t add(int argc, mal_t *argv);
mal_t sub(int argc, mal_t *argv);
mal_t mul(int argc, mal_t *argv);
mal_t divide(int argc, mal_t *argv);
mal_t eq(int argc, mal_t *argv);
mal_t lt(int argc, mal_t *argv);
mal_t lte(int argc, mal_t *argv);
mal_t gt(int argc, mal_t *argv);
mal_t gte(int argc, mal_t *argv);
mal_t list(int argc, mal_t *argv);
mal_t is_list(int argc, mal_t *argv);
mal_t is_empty(int argc, mal_t *argv);
mal_t length(int argc, mal_t *argv);
mal_t print(int argc, mal_t *argv);

#endif