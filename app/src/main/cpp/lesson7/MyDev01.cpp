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
    envmap = new EnvMap();
    envmap2 = new EnvMap();

    // Object specification
    //string path = "/planeNsphere.";
    //string path = "/plant.";
    string path = "/maxPlanck.";
    string modelPath = "models" + path + "model";
    string coeffPath = "coefficients" + path + "coeff";
    obj->parser(modelPath.c_str(), coeffPath.c_str());

    // Environment map specification
    string envPath = "texture/test/equirectangular3_8040.png";
    envmap->renderToTexture(80, 40, envPath.c_str());
    envmap->obj = obj;

    string envPath2 = "texture/bloodMoon/equirectangular_6432.png";
    envmap2->renderToTexture(64, 32, envPath2.c_str());
    envmap2->obj = obj;

    int order = 4;

    envmap->setLightCoeff(order);
    envmap2->setLightCoeff(order);

    // Skybox specification

//    const char * paths[6] = {"texture/test/annotated/right.png", "texture/test/annotated/left.png", "texture/test/annotated/top.png",
//                             "texture/test/annotated/bottom.png", "texture/test/annotated/near.png", "texture/test/annotated/far.png"};

        const char * paths[6] = {"texture/test/annotated/right.png", "texture/test/annotated/left.png", "texture/test/annotated/top.png",
                             "texture/test/annotated/bottom.png", "texture/test/annotated/near.png", "texture/test/annotated/far.png"};

        const char * paths2[6] = {"texture/bloodMoon/right.png", "texture/bloodMoon/left.png", "texture/bloodMoon/top.png",
                             "texture/bloodMoon/bottom.png", "texture/bloodMoon/near.png", "texture/bloodMoon/far.png"};

//    const char * paths[6] = {"texture/test/blackbox.png", "texture/test/blackbox.png", "texture/test/blackbox.png",
//     "texture/test/blackbox.png", "texture/test/blackbox.png", "texture/test/far.png"};

//    const char * paths[6] = {"texture/graceCathedral/right.png", "texture/graceCathedral/left.png", "texture/graceCathedral/top.png",
//                             "texture/graceCathedral/bottom.png", "texture/graceCathedral/near.png", "texture/graceCathedral/far.png"};

    skybox->setSkybox(paths);
    skybox2->setSkybox(paths2);
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

    //LOGD("%d %d", mProgramHandle, mSkyProgramHandle);

    obj->initialize(mProgramHandle);
    skybox->initialize(mSkyProgramHandle);
    skybox2->initialize(mSkyProgramHandle2);

    mModelMatrix = new Matrix();
    mModelMatrix->identity();

    mProjectionMatrix = new Matrix();
    mCurrentRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix->identity();

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
    //time += 0.01f;
    //currentTime = clock();
    //updateFPS(time);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do a compile rotation every 10 seconds;
    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 100000.0f) * ((int) time);

    // Draw a cube.
    // Translate the cube into the screen.
    //mModelMatrix->identity();
    //mModelMatrix->translate(0.0f, 0.0f, -3.5f);

    // Touch rotation
    mCurrentRotationMatrix->identity();
    mCurrentRotationMatrix->rotateLocal(mDeltaX, 0.0f, 1.0f, 0.0f);
    mCurrentRotationMatrix->rotateLocal(mDeltaY, 1.0f, 0.0f, 0.0f);

    mDeltaX = 0.0f;
    mDeltaY = 0.0f;
    mAccumulatedRotationMatrix->multiply(*mCurrentRotationMatrix, *mAccumulatedRotationMatrix);
    mAccumulatedRotationMatrix->transpose(*inverseRotationMatrix, *mAccumulatedRotationMatrix);

    rotation();

    // Set object and skybox V and P
//    obj->mViewMatrix.multiply(*mViewMatrix, *mAccumulatedRotationMatrix);

    currentSkybox->renderer();
    obj->renderer();

}

void MyDev01::setRotationMode(bool val)
{
    isSky = val;

    mAccumulatedRotationMatrix->identity();
    inverseRotationMatrix->identity();
    obj->mModelMatrix.identity();
    currentSkybox->mModelMatrix.identity();
}

void MyDev01::rotation()
{
    if(isSky)
    {
        currentEnv->updateLightCoeff(*inverseRotationMatrix);
        currentSkybox->mModelMatrix = *mAccumulatedRotationMatrix;
    }
    else
    {
        currentEnv->updateLightCoeff(*mAccumulatedRotationMatrix);
        obj->mModelMatrix = *mAccumulatedRotationMatrix;
    }
    obj->mViewMatrix = *mViewMatrix;
    obj->mProjectionMatrix = *mProjectionMatrix;
    skybox->mViewMatrix = *mViewMatrix;
    skybox->mProjectionMatrix = *mProjectionMatrix;
    skybox2->mViewMatrix = *mViewMatrix;
    skybox2->mProjectionMatrix = *mProjectionMatrix;
}

void MyDev01::setSkybox(bool val)
{
    if(val)
    {
        currentSkybox = skybox;
        currentEnv = envmap;
    }
    else
    {
        currentSkybox = skybox2;
        currentEnv = envmap2;
    }


}

void MyDev01::setDelta(float x, float y)
{
    mDeltaX = x;
    mDeltaY = y;
}



