#ifndef GRAMMER_H
#define GRAMMER_H

#include <regex.h>

#define NUM_TOKENS_DEFS 1

typedef struct grammerdef {
    char *pattern;
    char *token;
    regex_t comp_regex;
} grammerdef;

extern grammerdef grammerdefs[];

#endif