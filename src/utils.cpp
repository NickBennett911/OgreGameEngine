#include <stdafx.h>
#include <utils.h>


float ssuge::RandomFloat(float a, float b) {
    assert(b > a);
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}