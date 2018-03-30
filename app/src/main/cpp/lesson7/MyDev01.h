//
// Created by karaianas on 2/17/2018.
//

#ifndef ANDROIDOPENGLESLESSONSCPP_MYDEV01_H
#define ANDROIDOPENGLESLESSONSCPP_MYDEV01_H

#include <GLES2/gl2.h>
#include <graphics/Matrix.h>
#include <graphics/GLUtils.h>
#include <android/log.h>

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <cstddef>
#include <iostream>
#include <string>

#include "EnvMap.h"

#define LOG_TAG "MyDev01"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#include "Obj.h"
#include "Skybox.h"

class MyDev01{

public:

    MyDev01();
    ~MyDev01();

    void create();
    void change(int width, int height);
    void draw();

    void setRotationMode(bool val);

    void setSkybox(int val);

    void reset();

    void setDelta(float x, float y);

    bool isSky;

    Obj* obj;
    Skybox* currentSkybox;
    Skybox* skybox;
    Skybox* skybox2;
    Skybox* skybox3;
    EnvMap* currentEnv;
    EnvMap* envmap;
    EnvMap* envmap2;
    EnvMap* envmap3;

    // model/view/projection matrix
    Matrix *mModelMatrix;
    Matrix *mViewMatrix;
    Matrix *mProjectionMatrix;

    //
    Matrix *mAccumulatedRotationMatrix;
    Matrix *mAccumulatedRotationMatrix2;
    Matrix *mCurrentRotationMatrix;
    Matrix *inverseRotationMatrix;

    GLuint mProgramHandle;
    GLuint mSkyProgramHandle;
    GLuint mSkyProgramHandle2;
    GLuint mSkyProgramHandle3;

    float mDeltaX;
    float mDeltaY;
};


#endif //ANDROIDOPENGLESLESSONSCPP_MYDEV01_H
