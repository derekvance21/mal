#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

typedef struct token {
    int so;
    int eo;
    int len;
} token_t;

typedef struct tokens {
    int num;
    token_t *tokens;
    char *str;
} tokens_t;

tokens_t* tokenize(char *in) {
    regex_t regex;
    int reti;
    regmatch_t pmatch[2];
    char errbuf[1024];
    tokens_t *tokens = malloc(sizeof(tokens_t));
    tokens->num = 0;
    tokens->tokens = calloc(tokens->num, sizeof(token_t));
    tokens->str = in;
    
    int tokens_len = 1;
    size_t in_len = strlen(in);

    // withing capture group, curly braces need to be escaped or regcomp will yell at you
    // ^^^ HOWEVER, this doesn't actually work, doing '\\{\\}' inside a character class will match backslashes
    // ^^^ DOUBLE HOWEVER, when it yells at you 'Invalid content of \{\}', it goes away if you remove the literal square brackets (WHAT?)
    // for some reason, within symbol character class, \\[]\\[] must appear before or after the ) , and nowhere else. :|
    // missing piece is string literals, specifically capturing the entire literal as one capture group (while still supporting escaped double quotes) inside string
    // there's something wrong with the non-capturing group. hmmm. 'Could not compile regex: Invalid preceding regular expression'
    // "[[:space:],]*(~@|[\\[\\]{}()'`~^@]|\"(?:\\\\.|[^\\\\\"])*\"?|;.*|[^[:space:]\\[\\]{}('\"`,;)]*)"
    reti = regcomp(&regex, "[[:space:],]*(~@|[{}()'`~^@]|\"(\\\\.|[^\\\"])*\"?|;.*|[^[:space:]{}('\"`,;)]*)", REG_EXTENDED);

    // if I leave the \\[\\] (or '[]', it doesn't matter) it won't match correctly at all 
    // for some reason, \\s* will match whitespace, but [\\s]* will not!
    // even though [def] will match any of the characters d, e, and f
    // I think it's because backslashing within character groups fucks things up, so it's treated as '\' and 's'
    // so have to use [:space:]
    // reti = regcomp(&regex, "d", REG_EXTENDED);
    
    if (reti) {
        regerror(reti, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Could not compile regex: %s\n", errbuf);
        exit(1);
    }
    
    int i;
    for (i = 0; i < in_len; i=i) {
        reti = regexec(&regex, in + i, 2, pmatch, 0);
    
        if (reti) {
            regerror(reti, &regex, errbuf, sizeof(errbuf));
            fprintf(stderr, "Could not execute regex: %s\n", errbuf);
            exit(1);
        }

	// pmatch[0] == consumed characters
	// pmatch[1] == captured token
        
	token_t token;
	token.so = i + pmatch[1].rm_so;
	token.eo = i + pmatch[1].rm_eo;
	token.len = pmatch[1].rm_eo - pmatch[1].rm_so;
	tokens->tokens[tokens->num++] = token;
	
	
	if (tokens->num >= tokens_len) {
	    printf("reallocing from %d to %d size\n", tokens_len, tokens_len * 2);
	    tokens->tokens = reallocarray(tokens->tokens, tokens_len *= 2, sizeof(token_t));
	}

	i += pmatch[1].rm_eo;
    }

    regfree(&regex);
    tokens->tokens = reallocarray(tokens->tokens, tokens->num, sizeof(token_t));
    return tokens;
}

void freeTokens(tokens_t *tokens) {
    free(tokens->tokens);
    free(tokens);
}

int main(int argc, char *argv[]) {
    char *line = "  (+ 3 4 (add 23 1) (mult 4 5) {length \"derek vance \\\"the great\\\" - cheesy\"})";
    // char *line = "d";
    printf("%s\n", line);
    tokens_t *tokens = tokenize(line);
    
    char token[1024];
    int i;
    for (i = 0; i < tokens->num; ++i) {
	memset(token, '\0', sizeof(token));
        strncpy(token, tokens->str + tokens->tokens[i].so, tokens->tokens[i].len);        
	printf("token %d: %s|\n", i, token);
    }

    freeTokens(tokens);
}
