//
// Created by karaianas on 2/17/2018.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_SKYBOX_H
#define ANDROIDOPENGLESLESSONSCPP_SKYBOX_H

#include <vector>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <graphics/GLUtils.h>
#include <string>
#include <graphics/Matrix.h>

using namespace std;

class Skybox{

public:
    Skybox();

    void initialize(GLuint mProgramHandle);
    void renderer();

//private:

    GLuint program;

    Matrix mModelMatrix;
    Matrix mViewMatrix;
    Matrix mProjectionMatrix;
    Matrix mMVPMatrix;
    Matrix mMVMatrix;

    GLuint mPositionHandle = 0;
    GLuint mSkyboxHandle = 0;
    GLuint mMVPMatrixHandle = 0;
    GLuint mMVMatrixHandle = 0;

    GLuint mPositionsBufferIdx;
    GLuint mSkyboxBufferIdx;

    GLuint offscreen_framebuffer, onscreen_framebuffer;

vector<float> positions{
            // Front face
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Right face
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,

            // Back face
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            // Left face
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,

            // Top face
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,

            // Bottom face
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
    };

};
#endif //ANDROIDOPENGLESLESSONSCPP_SKYBOX_H
