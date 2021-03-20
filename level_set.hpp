#pragma once

#include "gef.hpp"

#define MAX_LEVELS_IN_SET 10

typedef struct {
    const char *name;
    colour background;
    colour highlight;
    const char *levels[MAX_LEVELS_IN_SET];
    bool completed[MAX_LEVELS_IN_SET];
    int num_levels;
    int total_presents;
    int collected_presents;
} level_set;