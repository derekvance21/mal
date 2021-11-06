#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

struct reader;

typedef struct reader {
    regex_t regex;
    regmatch_t *tokens;
    int pos;
    regmatch_t* (*next)(struct reader *self, char *in);
    regmatch_t* (*peek)(struct reader *self, char *in);
} reader_t;

regmatch_t* next(reader_t *self, char *in) {
    return NULL;
}

regmatch_t* peek(reader_t *self, char *in) {
    return NULL;
}

void readerInit(reader_t *reader) {
    reader->next = next;
    reader->peek = peek;

}

regmatch_t* tokenize(char *in) {
    regex_t regex;
    int reti;
    // this should prolly be pmatch[1] - every call to regexec only expects 1 capture group
    regmatch_t pmatch[2];
    char errbuf[1024];

    // withing capture group, curly braces need to be escaped or regcomp will yell at you
    // ^^^ HOWEVER, this doesn't actually work, doing '\\{\\}' inside a character class will match backslashes
    // ^^^ DOUBLE HOWEVER, when it yells at you 'Invalid content of \{\}', it goes away if you remove the literal square brackets (WHAT?)
    // for some reason, within symbol character class, \\[]\\[] must appear before or after the ) , and nowhere else. :|
    // missing piece is string literals, specifically capturing the entire literal as one capture group (while still supporting escaped double quotes) inside string
    // there's something wrong with the non-capturing group. hmmm. 'Could not compile regex: Invalid preceding regular expression'
    // "[[:space:],]*(~@|[\\[\\]{}()'`~^@]|\"(?:\\\\.|[^\\\\\"])*\"?|;.*|[^[:space:]\\[\\]{}('\"`,;)]*)"
    // reti = regcomp(&regex, "[[:space:],]*(~@|[{}()'`~^@]|\"(\\\\.|[^\\\"])*\"?|;.*|[^[:space:]{}('\"`,;)]*)", REG_EXTENDED);

    // if I leave the \\[\\] (or '[]', it doesn't matter) it won't match correctly at all 
    // for some reason, \\s* will match whitespace, but [\\s]* will not!
    // even though [def] will match any of the characters d, e, and f
    // I think it's because backslashing within character groups fucks things up, so it's treated as '\' and 's'
    // so have to use [:space:]
    reti = regcomp(&regex, "d", REG_EXTENDED);
    
    if (reti) {
        regerror(reti, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Could not compile regex: %s\n", errbuf);
        exit(1);
    }
    printf("%s\n", in);
    reti = regexec(&regex, in, 2, pmatch, 0);
    if (reti) {
        regerror(reti, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Could not execute regex: %s\n", errbuf);
        exit(1);
    }

    int i;
    for (i = 0; i < 2; ++i) {
        if (pmatch[i].rm_so == -1 && pmatch[i].rm_so == -1) {
            break;
        }
        printf("%d) so: %d, eo: %d\n", i, pmatch[i].rm_so, pmatch[i].rm_eo);
    }
    regfree(&regex);
}

int main(int argc, char *argv[]) {
    // char *line = "  (+ 3 4 (add 23 1) [mult 4 5] {length \"derek vance \\\"the great\\\" - cheesy\"})";
    char *line = "d";
    tokenize(line);
}
