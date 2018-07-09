#ifndef TOKEN_H
#define TOKEN_H

#include <regex.h>

#include "array_buffer_helpers.h"

#define NUM_TOKENS_DEFS 14
#define CLASS "CLASS"
#define END "END"
#define EOL "EOL"
#define EQUAL "="
#define FLOAT "FLOAT"
#define INT "INT"
#define NUMBER "NUMBER"
#define PAREN_CLOSE "PAREN_CLOSE"
#define PAREN_OPEN "PAREN_OPEN"
#define PRINT "PRINT"
#define STR "STR"
#define STRING_LITERAL "STRING_LITERAL"
#define VAR "VAR"
#define WHITESPACE "WHITESPACE"

typedef struct tokendef {
    char *pattern;
    char *token;
    char capture;
    unsigned int token_len;
    regex_t comp_regex;
} tokendef;

// Public
char compile_regexes();
char tokenize(char *str, buffer *token_buffer, string_array *token_values);

// Private
extern tokendef tokendefs[];
char append_token_value(string_array *array, char *str, unsigned int char_offset, unsigned int char_len);
char append_buffer(buffer *buff, char *str, char token_len, int token_num);
char get_append_str(char *str, char token_len, int token_num, char **append_str, unsigned int *append_str_len);

#endif