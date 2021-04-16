#pragma once

#include <stdint.h>

int util_rand_ab(int a, int b);
uint64_t get_us();




float rand_floatn(float min, float max);
float hash_floatn(int position, float min, float max);
float hash_intn(int position, int min, int max);
char *slurp(const char *path);