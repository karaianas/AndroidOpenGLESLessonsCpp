//
// Created by karaianas on 2/18/2018.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_ENVMAP_H
#define ANDROIDOPENGLESLESSONSCPP_ENVMAP_H

#include <vector>
#include <GLES2/gl2.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <graphics/GLUtils.h>

#include <string>
#include <ctype.h>
#include <math.h>
#include <cstdlib>
#include <graphics/Matrix.h>

using namespace std;

class EnvMap{
public:
    vector<float> getPixel(int i, int j);
    void parser();

    int width, height;
    vector<vector<float>> colors;
};
#endif //ANDROIDOPENGLESLESSONSCPP_ENVMAP_H
