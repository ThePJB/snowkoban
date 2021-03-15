#pragma once

#include "gef.h"

#define MAX_LEVELS_IN_SET 10

typedef struct {
    char *name;
    char *levels[MAX_LEVELS_IN_SET];
    bool completed[MAX_LEVELS_IN_SET];
    colour background;
    colour highlight;
    int num_levels;
    int total_presents;
    int collected_presents;
} level_set;