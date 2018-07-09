#include "grammer.h"

grammerdef grammerdefs[] = {
    {
        .pattern = "[ \t]+",
        .token = "WHITESPACE",
        .comp_regex = 0
    }
};