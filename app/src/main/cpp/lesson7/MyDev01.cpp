//
// Created by karaianas on 2/17/2018.
//

#include "MyDev01.h"

MyDev01::MyDev01()
{
    mViewMatrix = nullptr;
    mModelMatrix = nullptr;
    mProjectionMatrix = nullptr;
    mMVPMatrix = nullptr;

    mMVPMatrixHandle = 0;
    mMVMatrixHandle = 0;

    obj = new Obj();
    obj->parser();
    skybox = new Skybox();
}

MyDev01::~MyDev01()
{
    delete mModelMatrix;
    mModelMatrix = nullptr;
    delete mViewMatrix;
    mViewMatrix = nullptr;
    delete mProjectionMatrix;
    mProjectionMatrix = nullptr;
    delete mMVPMatrix;
    mMVPMatrix = nullptr;
}

void MyDev01::updateFPS(float fps)
{

}

void MyDev01::create()
{
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
    if (!mProgramHandle)
    {
        LOGD("Could not create program");
        return;
    }

    mSkyProgramHandle = GLUtils::createProgram(&vertex2, &fragment2);
    if (!mSkyProgramHandle)
    {
        LOGD("Could not create skybox program");
        return;
    }

    //LOGD("%d %d", mProgramHandle, mSkyProgramHandle);

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
}

void MyDev01::change(int width, int height)
{
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
}

void MyDev01::draw()
{
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
}

void MyDev01::setDelta(float x, float y)
{
    mDeltaX = x;
    mDeltaY = y;
}



