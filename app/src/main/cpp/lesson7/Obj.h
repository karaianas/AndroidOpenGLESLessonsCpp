//
// Created by karaianas on 2/14/2018.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_OBJ_H
#define ANDROIDOPENGLESLESSONSCPP_OBJ_H

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

class Obj{

public:

    Obj();

    void initialize(GLuint mProgramHandle);
    void renderer();
    void parser();

    GLuint program;

    Matrix mModelMatrix;
    Matrix mViewMatrix;
    Matrix mProjectionMatrix;
    Matrix mMVPMatrix;
    Matrix mMVMatrix;

    GLuint mPositionHandle = 0;
    GLuint mNormalHandle = 0;
    GLuint mCoeffHandle = 0;
    GLuint mMVPMatrixHandle = 0;
    GLuint mMVMatrixHandle = 0;

    GLuint mPositionsBufferIdx;
    GLuint mNormalsBufferIdx;
    GLuint mIndexBufferIdx;
    GLuint mCoeffsBufferIdx;

    vector<float> positions;
    vector<float> normals;
    vector<short> indices;
    vector<float> coeffs;

public:

    static int BYTES_PER_FLOAT;
    static int BYTES_PER_SHORT;
    static int  POSITION_STEP;
    static int  NORMAL_STEP;
    static int  COEFF_STEP;
};


#endif //ANDROIDOPENGLESLESSONSCPP_OBJ_H
