//
// Created by biezhihua on 2017/7/22.
//

#include <graphics/GLUtils.h>
#include "Native7Lesson.h"
#include "GenData.h"
#include <android/log.h>

//#include "Obj.h"
#include "MyDev01.h"
//#include "Skybox.h"

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
    /*
    mViewMatrix = nullptr;
    mModelMatrix = nullptr;
    mProjectionMatrix = nullptr;
    mMVPMatrix = nullptr;

    mMVPMatrixHandle = 0;
    mMVMatrixHandle = 0;

    obj = new Obj();
    obj->parser();
    skybox = new Skybox();
     */
}

Native7Lesson::~Native7Lesson() {

    delete mydev01;
    /*
    delete mModelMatrix;
    mModelMatrix = nullptr;
    delete mViewMatrix;
    mViewMatrix = nullptr;
    delete mProjectionMatrix;
    mProjectionMatrix = nullptr;
    delete mMVPMatrix;
    mMVPMatrix = nullptr;
     */
}

void Native7Lesson::create() {

    mydev01->create();
    /*
    // Set the background clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = 0.0f;
    float eyeZ = 5.0f;

    // We are looking at the origin
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;

    // Set our up vector.
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    // Set the view matrix.
    mViewMatrix = new Matrix();
    mViewMatrix = Matrix::newLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

    const char *vertex = GLUtils::openTextFile("vertex/mydev_test_vertex_shader.glsl");
    const char *fragment = GLUtils::openTextFile("fragment/mydev_test_fragment_shader.glsl");

    const char *vertex2 = GLUtils::openTextFile("vertex/mydev_skybox_vertex_shader.glsl");
    const char *fragment2 = GLUtils::openTextFile("fragment/mydev_skybox_fragment_shader.glsl");

    // Set program handles
    mProgramHandle = GLUtils::createProgram(&vertex, &fragment);
    if (!mProgramHandle) {
        LOGD("Could not create program");
        return;
    }

    mSkyProgramHandle = GLUtils::createProgram(&vertex2, &fragment2);
    if (!mSkyProgramHandle) {
        LOGD("Could not create skybox program");
        return;
    }

    LOGD("%d %d", mProgramHandle, mSkyProgramHandle);

    obj->initialize(mProgramHandle);
    skybox->initialize(mSkyProgramHandle);


    mModelMatrix = new Matrix();
    mModelMatrix->identity();

    mMVPMatrix = new Matrix();
    mProjectionMatrix = new Matrix();
    mCurrentRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix->identity();

    mDeltaX = 0.0f;
    mDeltaY = 0.0f;
     */
}

void Native7Lesson::change(int width, int height) {

    mydev01->change(width, height);
    /*
    glViewport(0, 0, width, height);

    // Create a new perspective projection matrix. The height will stay the same
    // while the width will vary as per aspect ratio.
    float scaleFactor = 10.0f;
    float ratio = (float) width / height;
    float left = -ratio;// * scaleFactor;
    float right = ratio;// * scaleFactor;
    float bottom = -1.0f;// * scaleFactor;
    float top = 1.0f;// * scaleFactor;
    float near = 1.0f;// * scaleFactor;
    float far = 100.0f;// * scaleFactor;

    mProjectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);
     */
}

void Native7Lesson::draw()
{
    mydev01->draw();
    /*
    //time += 0.01f;
    currentTime = clock();
    //updateFPS(time);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do a compile rotation every 10 seconds;
    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 100000.0f) * ((int) time);

    // Set program handles for cube drawing.
    mMVPMatrixHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(mProgramHandle, "u_MVMatrix");
    GLuint positionHandle = (GLuint) glGetAttribLocation(mProgramHandle, "a_Position");
    GLuint normalHandle = (GLuint) glGetAttribLocation(mProgramHandle, "a_Normal");
    GLuint coeffHandle = (GLuint) glGetAttribLocation(mProgramHandle, "a_Coeff");

    GLuint positionHandle2 = (GLuint) glGetAttribLocation(mSkyProgramHandle, "a_Position");
    mMVPMatrixHandle2 = (GLuint) glGetUniformLocation(mSkyProgramHandle, "u_MVPMatrix");
    mMVMatrixHandle2 = (GLuint) glGetUniformLocation(mSkyProgramHandle, "u_MVMatrix");

    obj->mPositionHandle = positionHandle;
    obj->mNormalHandle = normalHandle;
    obj->mCoeffHandle = coeffHandle;
    obj->mMVMatrixHandle = mMVMatrixHandle;
    obj->mMVPMatrixHandle = mMVPMatrixHandle;

    skybox->mPositionHandle = positionHandle2;
    skybox->mMVPMatrixHandle = mMVPMatrixHandle2;
    skybox->mMVMatrixHandle = mMVMatrixHandle2;


    // Draw a cube.
    // Translate the cube into the screen.
    //mModelMatrix->identity();
    //mModelMatrix->translate(0.0f, 0.0f, -3.5f);
    //mModelMatrix->rotate(angleInDegrees, 1, 1, 1);

    // Set a matrix that contains the current rotation.
    mCurrentRotationMatrix->identity();
    mCurrentRotationMatrix->rotate(mDeltaX, 0.0f, 1.0f, 0.0f);
    mCurrentRotationMatrix->rotate(mDeltaY, 1.0f, 0.0f, 0.0f);
    mDeltaX = 0.0f;
    mDeltaY = 0.0f;
    mModelMatrix->multiply(*mCurrentRotationMatrix, *mModelMatrix);


    mMVPMatrix->multiply(*mViewMatrix, *mModelMatrix);
    glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, mMVPMatrix->mData);
    obj->mMVMatrix = mMVPMatrix;

    glUniformMatrix4fv(mMVMatrixHandle2, 1, GL_FALSE, mMVPMatrix->mData);
    skybox->mMVMatrix = mMVPMatrix;

    mMVPMatrix->multiply(*mProjectionMatrix, *mMVPMatrix);
    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, mMVPMatrix->mData);
    obj->mMVPMatrix = mMVPMatrix;

    glUniformMatrix4fv(mMVPMatrixHandle2, 1, GL_FALSE, mMVPMatrix->mData);
    skybox->mMVPMatrix = mMVPMatrix;

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(mSkyProgramHandle);
    skybox->renderer(mSkyProgramHandle);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(mProgramHandle);
    obj->renderer(mProgramHandle);

//    float lastDeltaTime = CLOCKS_PER_SEC/deltaTime;
//    deltaTime = clock() - currentTime;
//    float currentDeltaTime = CLOCKS_PER_SEC/(deltaTime);
//    if(abs(int(lastDeltaTime) - int(currentDeltaTime)) > 1)
//        updateFPS(currentDeltaTime);
     */
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
