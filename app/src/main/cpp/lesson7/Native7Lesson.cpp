//
// Created by biezhihua on 2017/7/22.
//

#include <graphics/GLUtils.h>
#include "Native7Lesson.h"
#include "GenData.h"
#include <android/log.h>

#include "MyDev01.h"

#define LOG_TAG "Lesson"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

// processing callback to handler class
typedef struct context {
    JavaVM *javaVM;
    jclass nativeRendererClz;
    jobject nativeRendererObj;
} Context;

static Native7Lesson *lesson7;
static GenData *genData;
static Context g_ctx;

MyDev01* mydev01;

Native7Lesson::Native7Lesson() {

    mydev01 = new MyDev01();

}

Native7Lesson::~Native7Lesson() {

    delete mydev01;

}

void Native7Lesson::create() {

    mydev01->create();

}

void Native7Lesson::change(int width, int height) {

    mydev01->change(width, height);

}

void Native7Lesson::draw()
{
    mydev01->draw();
    updateFPS(10.0f);
}

void Native7Lesson::decreaseCubeCount() {
    if (genData != nullptr && genData->mLastRequestedCubeFactor > 1) {
        genData->genCube(--genData->mLastRequestedCubeFactor, false, false);
    }
}

void Native7Lesson::increaseCubeCount() {
    if (genData != nullptr && genData->mLastRequestedCubeFactor < 16) {
        genData->genCube(++genData->mLastRequestedCubeFactor, false, false);
    }
}

void Native7Lesson::setDelta(float x, float y) {
    mDeltaX += x;
    mDeltaY += y;

    mydev01->setDelta(x, y);
}

void Native7Lesson::toggleStride() {
    genData->genCube(genData->mLastRequestedCubeFactor, false, true);
}

void Native7Lesson::toggleVBOs() {
    genData->genCube(genData->mLastRequestedCubeFactor, true, false);
}

void Native7Lesson::updateVboStatus(bool useVbos) {
    LOGD("updateVboStatus %d", useVbos);
    Context *pctx = &g_ctx;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = javaVM->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (JNI_OK != res) {
        LOGE("Failed to Get env, ErrorCode = %d", res);
        return;
    }
    jmethodID statusId = env->GetMethodID(pctx->nativeRendererClz, "updateVboStatus", "(Z)V");
    env->CallVoidMethod(pctx->nativeRendererObj, statusId, useVbos);
}

void Native7Lesson::updateStrideStatus(bool useStride) {
    LOGD("updateStrideStatus %d", useStride);
    Context *pctx = &g_ctx;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = javaVM->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (JNI_OK != res) {
        LOGE("Failed to Get env, ErrorCode = %d", res);
        return;
    }
    jmethodID statusId = env->GetMethodID(pctx->nativeRendererClz, "updateStrideStatus", "(Z)V");
    env->CallVoidMethod(pctx->nativeRendererObj, statusId, useStride);
}

// ++++++++++++++++++++++++
void Native7Lesson::updateFPS(float fps)
{
    LOGD("FPS %2.2f", fps);
    Context *pctx = &g_ctx;
    JavaVM *javaVM = pctx->javaVM;
    JNIEnv *env;
    jint res = javaVM->GetEnv((void **) &env, JNI_VERSION_1_6);
    if (JNI_OK != res) {
        LOGE("Failed to Get env, ErrorCode = %d", res);
        return;
    }
    jmethodID statusId = env->GetMethodID(pctx->nativeRendererClz, "updateFPS", "(F)V");
    env->CallVoidMethod(pctx->nativeRendererObj, statusId, fps);
}
// ++++++++++++++++++++++++

// ----------------------------------------------------------

extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    memset(&g_ctx, 0, sizeof(g_ctx));

    g_ctx.javaVM = vm;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR; // JNI version not supported.
    }
    g_ctx.nativeRendererObj = NULL;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeToggleStride(JNIEnv *env,
                                                                                    jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->toggleStride();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeToggleVBOs(JNIEnv *env,
                                                                                  jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->toggleVBOs();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeIncreaseCubeCount(
        JNIEnv *env, jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->increaseCubeCount();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeDecreaseCubeCount(
        JNIEnv *env, jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->decreaseCubeCount();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeSetDelta(JNIEnv *env,
                                                                                jobject instance,
                                                                                jfloat x,
                                                                                jfloat y) {
    if (lesson7 != nullptr) {
        lesson7->setDelta(x, y);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeDrawFrame(JNIEnv *env,
                                                                                 jobject instance) {
    if (lesson7 != nullptr) {
        lesson7->draw();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeSurfaceChange(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jint width,
                                                                                     jint height) {
    if (lesson7 != nullptr) {
        lesson7->change(width, height);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeSurfaceCreate(JNIEnv *env,
                                                                                     jobject instance,
                                                                                     jobject assetManager) {

    GLUtils::setEnvAndAssetManager(env, assetManager);
    lesson7 = new Native7Lesson();
    genData = new GenData(lesson7);
    if (lesson7 != nullptr) {
        lesson7->create();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeDestroy(JNIEnv *env,
                                                                               jobject instance) {
    env->DeleteGlobalRef(g_ctx.nativeRendererClz);
    env->DeleteGlobalRef(g_ctx.nativeRendererObj);
    g_ctx.nativeRendererClz = NULL;
    g_ctx.nativeRendererObj = NULL;

    if (lesson7 != nullptr) {
        delete (lesson7);
        lesson7 = NULL;
    }
    if (genData != nullptr) {
        delete (genData);
        genData = NULL;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_learnopengles_android_lesson7_LessonSevenNativeRenderer_nativeInit(JNIEnv *env,
                                                                            jobject instance) {
    jclass clz = env->GetObjectClass(instance);
    g_ctx.nativeRendererClz = (jclass) env->NewGlobalRef(clz);
    g_ctx.nativeRendererObj = env->NewGlobalRef(instance);

}
