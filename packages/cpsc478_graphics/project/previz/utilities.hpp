#pragma once

#include <assert.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "SETTINGS.h"

using namespace std;

extern Real CUSTOM_EPSILON;

Real generateEpsilon(Real scale = 10000.0);

Real degreesToRadians(Real degrees);

VEC3 clampVec3(VEC3 vec, Real low, Real high);

Real clamp(Real x, Real lower, Real upper);

float* allocatePPM(int xRes, int yRes);

void initPPM(float* values, int xRes, int yRes);

int indexIntoPPM(int x, int y, int xRes, int yRes,
                 bool originBottomLeft = false);

void writeColorToPPM(VEC3 color, float* ppm, int startIndex);

void readPPM(const string& filename, int& xRes, int& yRes, float*& values);

void writePPM(const string& filename, int& xRes, int& yRes,
              const float* values);

VEC3 truncate(const VEC4& v);
VEC4 extend(const VEC3& v);