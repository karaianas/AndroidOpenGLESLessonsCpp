//
// Created by biezhihua on 2017/7/22.
//

#ifndef OPENGLLESSON_NATIVE7LESSON_H
#define OPENGLLESSON_NATIVE7LESSON_H


#include <GLES2/gl2.h>
#include <graphics/Matrix.h>

// ++++++++++++++++++++++++
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <cstddef>
#include <iostream>
#include <string>

#include "Obj.h"
#include "Skybox.h"
// +++++++++++++++++++++++++++++

class Native7Lesson {
public:
    // ++++++++++++++++++++++++
    float time;
    clock_t currentTime;
    clock_t deltaTime;
    // ++++++++++++++++++++++++

    Native7Lesson();

    ~Native7Lesson();

    // ++++++++++++++++++++++++
    void updateFPS(float fps);
    // ++++++++++++++++++++++++

    void create();

    void change(int width, int height);

    void draw();

    void decreaseCubeCount();

    void increaseCubeCount();

    void setDelta(float x, float y);

    void toggleStride();

    void toggleVBOs();

    void updateVboStatus(bool useVbos);

    void updateStrideStatus(bool useStride);

private:

    bool isSkyboxRotation;
    bool skyboxMode;

    // model/view/projection matrix
    Matrix *mModelMatrix;
    Matrix *mViewMatrix;
    Matrix *mProjectionMatrix;
    Matrix *mMVPMatrix;

    //
    Matrix *mAccumulatedRotationMatrix;
    Matrix *mCurrentRotationMatrix;

    //
    GLuint mMVPMatrixHandle;
    GLuint mMVMatrixHandle;
    GLuint mMVPMatrixHandle2;
    GLuint mMVMatrixHandle2;

    GLuint mProgramHandle;
    GLuint mSkyProgramHandle;

    float mDeltaX;
    float mDeltaY;

    float previousTime;
};


#endif //OPENGLLESSON_NATIVE7LESSON_H
