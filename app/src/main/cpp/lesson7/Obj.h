//
// Created by karaianas on 2/14/2018.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_OBJ_H
#define ANDROIDOPENGLESLESSONSCPP_OBJ_H

#include <vector>
#include <GLES2/gl2.h>

// ++++++++++++++++++++++++
#include <android/log.h>
#include <GLES3/gl3.h>
// ++++++++++++++++++++++++

using namespace std;

class Obj{

private:
    GLuint mPositionsBufferIdx;
    GLuint mNormalsBufferIdx;
    GLuint mTexCoordsBufferIdx;

public:
    void test();
    void initialize(vector<float> *positions, vector<float> *normals, vector<float> *textureCoords);
};


#endif //ANDROIDOPENGLESLESSONSCPP_OBJ_H
