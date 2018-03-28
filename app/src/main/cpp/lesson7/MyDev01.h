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
    float time;
    clock_t currentTime;
    clock_t deltaTime;

    MyDev01();
    ~MyDev01();

    void updateFPS(float fps);

    void create();
    void change(int width, int height);
    void draw();

    void setSkybox(bool val);
    void changeSkybox();

    void setDelta(float x, float y);

    bool isSky;

    Obj* obj;
    Skybox* skybox;
    EnvMap* envmap;

    // model/view/projection matrix
    Matrix *mModelMatrix;
    Matrix *mViewMatrix;
    Matrix *mProjectionMatrix;

    //
    Matrix *mAccumulatedRotationMatrix;
    Matrix *mCurrentRotationMatrix;
    Matrix *inverseRotationMatrix;

    GLuint mProgramHandle;
    GLuint mSkyProgramHandle;

    float mDeltaX;
    float mDeltaY;
};


#endif //ANDROIDOPENGLESLESSONSCPP_MYDEV01_H
