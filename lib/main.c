#include <stdio.h>

#include "token.h"

int main(int argc, char **argv) {
    char *file_str;
    char load_status;
    buffer token_buffer = new_buffer();
    string_array token_values = new_string_array();

    if (argc != 2) {
        printf("Usage: boat <filename>\n");
        return -1;
    }

    load_status = load_file(argv[1], &file_str);
    if (load_status < 0) {
        printf("Failed to read file: %s\n", argv[1]);
        return -1;
    }

    tokenize(file_str, &token_buffer, &token_values);

    printf("Tokenized file:\n%s\n", token_buffer.str);
}