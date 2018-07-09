#ifndef ARRAYBUFFERHELPERS_H
#define ARRAYBUFFERHELPERS_H

typedef struct string_array {
    char **array;
    unsigned int total_len;
    unsigned int current_len;
} string_array;

typedef struct buffer {
    char *str;
    unsigned int total_len;
    unsigned int current_len;
} buffer;

string_array new_string_array();
buffer new_buffer();

#endif