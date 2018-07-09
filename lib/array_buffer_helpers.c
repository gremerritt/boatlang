#include <stdlib.h>

#include "array_buffer_helpers.h"

string_array new_string_array() {
    string_array tmp = {
        .array = NULL,
        .total_len = 0,
        .current_len = 0
    };

    return tmp;
}

buffer new_buffer() {
    buffer tmp = {
        .str = NULL,
        .total_len = 0,
        .current_len = 0
    };

    return tmp;
}