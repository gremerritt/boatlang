#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array_buffer_helpers.h"
#include "token.h"

tokendef tokendefs[] = {
    {
        .pattern = "[ \t]+",
        .token = WHITESPACE,
        .token_len = 10,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "int",
        .token = INT,
        .token_len = 3,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "float",
        .token = FLOAT,
        .token_len = 5,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "str",
        .token = STR,
        .token_len = 3,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "class",
        .token = CLASS,
        .token_len = 5,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "end",
        .token = END,
        .token_len = 3,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "=",
        .token = EQUAL,
        .token_len = 1,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)",
        .token = NUMBER,
        .token_len = 14,
        .comp_regex = 0,
        .capture = 1
    },
    {
        .pattern = "L?(\"(\\.|[^\\\"])*\"|'(\\.|[^\\\'])*')",
        .token = STRING_LITERAL,
        .token_len = 14,
        .comp_regex = 0,
        .capture = 1
    },
    {
        .pattern = "[A-Za-z_]+",
        .token = VAR,
        .token_len = 3,
        .comp_regex = 0,
        .capture = 1
    },
    {
        .pattern = "print",
        .token = PRINT,
        .token_len = 5,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "[(]",
        .token = PAREN_OPEN,
        .token_len = 10,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "[)]",
        .token = PAREN_CLOSE,
        .token_len = 11,
        .comp_regex = 0,
        .capture = 0
    },
    {
        .pattern = "(\r\n|\r|\n)",
        .token = EOL,
        .token_len = 3,
        .comp_regex = 0,
        .capture = 0
    },
};

static char regexes_compiled = 0;
char compile_regexes() {
    if (regexes_compiled) {
        return 0;
    }

    int i, ret_val;

    for(i=0; i<NUM_TOKENS_DEFS; i++) {
        tokendef *t = &tokendefs[i];

        ret_val = regcomp(&(t->comp_regex), t->pattern, REG_EXTENDED);
        
        if (ret_val) {
            printf("Error compiling regex %s\n", t->token);
            return -1;
        }
    }

    regexes_compiled = 1;

    return 0;
}

char tokenize(char *str, buffer *token_buffer, string_array *token_values) {
    char fnd;
    int str_len = strlen(str);
    int exec_err;
    unsigned char tokendef_index;
    unsigned long int char_offset = 0; 
    regmatch_t matches[1];

    if (compile_regexes()) {
        return -1;
    }

    while (1) {
        tokendef_index = 0;
        fnd = 0;

        if (char_offset >= str_len) {
            break;
        }

        for(tokendef_index=0; tokendef_index<NUM_TOKENS_DEFS; tokendef_index++) {
            exec_err = regexec(&tokendefs[tokendef_index].comp_regex, str + char_offset, 1, matches, 0);

            if (exec_err == REG_NOMATCH) {
                continue;
            } else if (exec_err) {
                printf("Parsing error [%i]. Quiting...\n", exec_err);
                return -2;
            } else if (matches[0].rm_so == 0 && matches[0].rm_eo != 0) {
                fnd = 1;
                if (tokendefs[tokendef_index].token != WHITESPACE) {
                    if (tokendefs[tokendef_index].capture) {
                        append_token_value(token_values, str, char_offset, matches[0].rm_eo);
                        append_buffer(token_buffer, tokendefs[tokendef_index].token, tokendefs[tokendef_index].token_len, (int)(token_values->current_len-1));
                    } else {
                        append_buffer(token_buffer, tokendefs[tokendef_index].token, tokendefs[tokendef_index].token_len, -1);
                    }
                }

                char_offset += matches[0].rm_eo;
                break;
            }
        }

        if (!fnd) {
            printf("Syntax error starting at: %s\n", str + char_offset);
            return -1;
        }
    }

    return 0;
}

char append_token_value(string_array *array, char *str, unsigned int char_offset, unsigned int char_len) {
    if (array->total_len == 0) {
        array->total_len = 2;
        array->current_len = 0;
        array->array = malloc( 2 * sizeof( char* ) );

        if (array->array == NULL) {
            printf("Initial array malloc failed\n");
            return -1;
        }
    } else if (array->current_len == array->total_len) {
        unsigned int new_len = array->total_len * 2;
        unsigned int i;

        // alloc the new array
        char **new_array = malloc( new_len * sizeof( char* ) );
        if (array->array == NULL) {
            printf("Temporary array malloc failed\n");
            return -1;
        }

        // copy the values
        for(i=0; i<array->total_len; i++) {
            new_array[i] = array->array[i];
        }

        // free the old array and re-point it
        free(array->array);
        array->array = new_array;
        array->total_len = new_len;
    }

    char *new_str = (char *) malloc( (char_len + 1) * sizeof(char) );

    if (new_str == NULL) {
        printf("String alloc failed\n");
        return -1;
    }

    strncpy(new_str, str + char_offset, char_len);
    new_str[char_len] = '\0';

    array->array[array->current_len++] = new_str;

    return 0;
}

char append_buffer(buffer *buff, char *str, char token_len, int token_num) {
    char *append_str;
    unsigned int append_str_len;
    unsigned int tmp_str_len;
    unsigned int new_str_len;
    char *tmp_str;

    unsigned int initial_len_mask = 1;
    unsigned int initial_len = 0;
    char i;

    char err = get_append_str(str, token_len, token_num, &append_str, &append_str_len);

    if (buff->str == NULL) {
        tmp_str_len = append_str_len + 2;  // +1 for the '\0' and +1 for the ' '

        // Set the initial buffer length to the next power of 2 big enough to hold the string
        for(i=0; i<(sizeof(unsigned int) * 8)-1; i++) {
            if (tmp_str_len & initial_len_mask) {
                initial_len = initial_len_mask * (initial_len ? 2 : 1);
            }

            initial_len_mask <<= 1;
        }

        buff->str = (char *)malloc( initial_len * sizeof(char) );

        if (buff->str == NULL) {
            printf("Initial token buffer malloc failed\n");
            return -1;
        }

        buff->total_len = initial_len;
        buff->current_len = 1;  // for the '\0'
    } else if (buff->current_len + append_str_len + 1 > buff->total_len) {  // +1 for the ' ' ('\0' already included)
        // If we can just double the length then do that.
        // If doubling still isn't big enough, adjust so it's big enough.
        if (buff->total_len * 2 >= buff->current_len + append_str_len + 1) {
            tmp_str_len = buff->total_len * 2;
        } else {
            tmp_str_len = buff->current_len + append_str_len + 1;  // +1 for the ' ' ('\0' already included)

            // Set the initial buffer length to the next power of 2 big enough to hold the string
            for(i=0; i<(sizeof(unsigned int) * 8)-1; i++) {
                if (tmp_str_len & initial_len_mask) {
                    initial_len = initial_len_mask * (initial_len ? 2 : 1);
                }

                initial_len_mask <<= 1;
            }

            tmp_str_len = initial_len;
        }

        tmp_str = (char *)malloc( tmp_str_len * sizeof(char) );

        if (tmp_str == NULL) {
            printf("Temporary token buffer malloc failed\n");
            return -1;
        }

        strncpy(tmp_str, buff->str, buff->current_len);  // this copies everything including the '\0'

        free(buff->str);
        buff->str = tmp_str;
        buff->total_len = tmp_str_len;
    }

    strncpy(buff->str + buff->current_len - 1, append_str, append_str_len);  // -1 to overwrite the '\0'
    buff->str[buff->current_len + append_str_len - 1] = ' ';
    buff->str[buff->current_len + append_str_len] = '\0';
    buff->current_len += append_str_len + 1;

    return 0;
}

char get_append_str(char *str, char token_len, int token_num, char **append_str, unsigned int *append_str_len) {
    unsigned int token_num_len = 0;
    int tmp_token_num = token_num;
    char err;

    while(tmp_token_num >= 0) {
        token_num_len++;
        tmp_token_num /= 10;

        if (tmp_token_num == 0) {
            break;
        }
    }

    *append_str_len = token_len + token_num_len;
    *append_str = (char *)malloc( (*append_str_len + 1) * sizeof(char) );

    if (*append_str == NULL) {
        return -1;
    }
    else if (token_num_len > 0) {
        err = sprintf(*append_str, "%s%i", str, token_num);
    } else {
        err = sprintf(*append_str, "%s", str);
    }

    return err;
}