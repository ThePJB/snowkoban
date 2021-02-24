#pragma once

float cm_floor(float x);
float cm_abs(float x);
float cm_frac(float x);
float cm_lerp(float a, float b, float t);
float cm_unlerp(float a, float b, float t);
float cm_remap(float prev_lower, float prev_upper, float new_lower, float new_upper, float a);