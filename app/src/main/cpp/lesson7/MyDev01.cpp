//
// Created by karaianas on 2/17/2018.
//

#include "MyDev01.h"
using namespace std;

MyDev01::MyDev01()
{
    mViewMatrix = nullptr;
    mModelMatrix = nullptr;
    mProjectionMatrix = nullptr;

    obj = new Obj();
    skybox = new Skybox();
    skybox2 = new Skybox();
    skybox3 = new Skybox();
    envmap = new EnvMap();
    envmap2 = new EnvMap();
    envmap3 = new EnvMap();

    // Object specification
//    string path = "/planeNsphere.";
    //string path = "/plant.";
    string path = "/maxPlanck.";
    string modelPath = "models" + path + "model";
    string coeffPath = "coefficients" + path + "coeff";
    obj->parser(modelPath.c_str(), coeffPath.c_str());

    // Environment map specification
    string envPath = "texture/test/equirectangular_8040_2.png";
    envmap->renderToTexture(80, 40, envPath.c_str());
    envmap->obj = obj;

    string envPath2 = "texture/sunset/equirectangular_6432_2.png";
    envmap2->renderToTexture(64, 32, envPath2.c_str());
    envmap2->obj = obj;

    string envPath3 = "texture/mountain/equirectangular_6432_2.png";
    envmap3->renderToTexture(64, 32, envPath3.c_str());
    envmap3->obj = obj;

    int order = 4;

    envmap->setLightCoeff(order);
    envmap2->setLightCoeff(order);
    envmap3->setLightCoeff(order);

    // Skybox specification
    const char * paths[6] = {"texture/test/annotated/right.png", "texture/test/annotated/left.png", "texture/test/annotated/top.png",
                             "texture/test/annotated/bottom.png", "texture/test/annotated/near.png", "texture/test/annotated/far.png"};

    const char * paths2[6] = {"texture/sunset/right.png", "texture/sunset/left.png", "texture/sunset/top.png",
                              "texture/sunset/bottom.png", "texture/sunset/near.png", "texture/sunset/far.png"};

    const char * paths3[6] = {"texture/mountain/right.png", "texture/mountain/left.png", "texture/mountain/top.png",
                             "texture/mountain/bottom.png", "texture/mountain/near.png", "texture/mountain/far.png"};

//    const char * paths[6] = {"texture/graceCathedral/right.png", "texture/graceCathedral/left.png", "texture/graceCathedral/top.png",
//                             "texture/graceCathedral/bottom.png", "texture/graceCathedral/near.png", "texture/graceCathedral/far.png"};

    skybox->setSkybox(paths);
    skybox2->setSkybox(paths2);
    skybox3->setSkybox(paths3);
    currentSkybox = skybox;
    currentEnv = envmap;
    isSky = true;
}

MyDev01::~MyDev01()
{
    delete mModelMatrix;
    mModelMatrix = nullptr;
    delete mViewMatrix;
    mViewMatrix = nullptr;
    delete mProjectionMatrix;
    mProjectionMatrix = nullptr;
}

void MyDev01::create()
{
    // Set the background clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Position the eye in front of the origin.
    float eyeX = 0.0f;
    float eyeY = -2.0f;
    float eyeZ = 10.0f;

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

    mSkyProgramHandle2 = GLUtils::createProgram(&vertex2, &fragment2);
    if (!mSkyProgramHandle2)
    {
        LOGD("Could not create skybox program");
        return;
    }

    mSkyProgramHandle3 = GLUtils::createProgram(&vertex2, &fragment2);
    if (!mSkyProgramHandle3)
    {
        LOGD("Could not create skybox program");
        return;
    }

    //LOGD("%d %d", mProgramHandle, mSkyProgramHandle);

    obj->initialize(mProgramHandle);
    skybox->initialize(mSkyProgramHandle);
    skybox2->initialize(mSkyProgramHandle2);
    skybox3->initialize(mSkyProgramHandle3);

    mModelMatrix = new Matrix();
    mModelMatrix->identity();

    mProjectionMatrix = new Matrix();
    mCurrentRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix->identity();
    mAccumulatedRotationMatrix2 = new Matrix();
    mAccumulatedRotationMatrix2->identity();

    inverseRotationMatrix = new Matrix();
    inverseRotationMatrix->identity();

    mDeltaX = 0.0f;
    mDeltaY = 0.0f;
}

void MyDev01::change(int width, int height)
{
    glViewport(0, 0, width, height);

    // Create a new perspective projection matrix. The height will stay the same
    // while the width will vary as per aspect ratio.
    float ratio = (float) width / height;
    float left = -ratio;
    float right = ratio;
    float bottom = -1.0f;
    float top = 1.0f;
    float near = 1.0f;
    float far = 100.0f;

    mProjectionMatrix = Matrix::newFrustum(left, right, bottom, top, near, far);
}

void MyDev01::draw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do a compile rotation every 10 seconds;
    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 100000.0f) * ((int) time);

    // Draw a cube.
    // Translate the cube into the screen.
    //mModelMatrix->identity();
    //mModelMatrix->translate(0.0f, 0.0f, -3.5f);
    //mViewMatrix->rotate(1, 0.0f, 1.0f, 0.0f);

    // Touch rotation
    if(isSky && mDeltaX && mDeltaY)
    {
        mCurrentRotationMatrix->identity();
        mCurrentRotationMatrix->rotate(mDeltaX, 0.0f, 1.0f, 0.0f);//rotateLocal
        mCurrentRotationMatrix->rotate(mDeltaY, 1.0f, 0.0f, 0.0f);

        mAccumulatedRotationMatrix->multiply(*mCurrentRotationMatrix, *mAccumulatedRotationMatrix);
        mAccumulatedRotationMatrix->transpose(*inverseRotationMatrix, *mAccumulatedRotationMatrix);
    }
    else if(!isSky)
    {
        mViewMatrix->rotate(0.5f, 0.0f, 1.0f, 0.0f);//
    }

    currentEnv->updateLightCoeff(*inverseRotationMatrix);
    currentSkybox->mModelMatrix = *mAccumulatedRotationMatrix;

    mDeltaX = 0.0f;
    mDeltaY = 0.0f;

    obj->mViewMatrix = *mViewMatrix;
    obj->mProjectionMatrix = *mProjectionMatrix;
    skybox->mViewMatrix = *mViewMatrix;
    skybox->mProjectionMatrix = *mProjectionMatrix;
    skybox2->mViewMatrix = *mViewMatrix;
    skybox2->mProjectionMatrix = *mProjectionMatrix;
    skybox3->mViewMatrix = *mViewMatrix;
    skybox3->mProjectionMatrix = *mProjectionMatrix;

    // Set object and skybox V and P
//    obj->mViewMatrix.multiply(*mViewMatrix, *mAccumulatedRotationMatrix);

    currentSkybox->renderer();
    obj->renderer();

}

void MyDev01::setRotationMode(bool val)
{
    isSky = val;
}

void MyDev01::setSkybox(int val)
{
    if(val == 0)
    {
        currentSkybox = skybox;
        currentEnv = envmap;
    }
    else if(val == 1)
    {
        currentSkybox = skybox2;
        currentEnv = envmap2;
    }
    else
    {
        currentSkybox = skybox3;
        currentEnv = envmap3;
    }

    draw();
}

void MyDev01::reset()
{
    mAccumulatedRotationMatrix->identity();
    inverseRotationMatrix->identity();

    float eyeX = 0.0f;
    float eyeY = -2.0f;
    float eyeZ = 10.0f;

    // We are looking at the origin
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;

    // Set our up vector.
    float upX = 0.0f;
    float upY = 1.0f;
    float upZ = 0.0f;

    mViewMatrix = Matrix::newLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);

    draw();
}

void MyDev01::setDelta(float x, float y)
{
    mDeltaX = x;
    mDeltaY = y;
}



