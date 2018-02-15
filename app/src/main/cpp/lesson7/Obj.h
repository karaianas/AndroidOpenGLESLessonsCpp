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

//private:
public:
    GLuint mPositionHandle = 0;
    GLuint mNormalHandle = 0;
    GLuint mTextureCoordHandle = 0;

private:
    GLuint mPositionsBufferIdx;
    GLuint mNormalsBufferIdx;
    GLuint mTexCoordsBufferIdx;

    vector<float> positions;
    vector<float> normals;
    vector<float> texCoords;
    vector<float> transfers;

public:
    void test();
    void initialize();
    void renderer();

    static int BYTES_PER_FLOAT;
    static int  POSITION_STEP;
    static int  NORMAL_STEP;
    static int  TEXCOORD_STEP;
    static int  TRANS_STEP;
};


#endif //ANDROIDOPENGLESLESSONSCPP_OBJ_H
