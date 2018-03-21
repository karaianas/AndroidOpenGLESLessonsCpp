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
    envmap = new EnvMap();

    // Object specification
    //string path = "/planeNsphere.";
    //string path = "/plant.";
    string path = "/maxPlanck.";
    string modelPath = "models" + path + "model";
    string coeffPath = "coefficients" + path + "coeff";
    obj->parser(modelPath.c_str(), coeffPath.c_str());

    // Environment map specification
    string envPath = "texture/bloodMoon/equirectangular_12864.png";
    envmap->renderToTexture(128, 64, envPath.c_str());
//    string envPath = "texture/test/equirectangular3_8040.png";
//    envmap->renderToTexture(80, 40, envPath.c_str());
    envmap->obj = obj;
    int order = 2;

    envmap->setLightCoeff(order);

    // Skybox specification
    const char * paths[6] = {"texture/bloodMoon/right.png", "texture/bloodMoon/left.png", "texture/bloodMoon/top.png",
                             "texture/bloodMoon/bottom.png", "texture/bloodMoon/near.png", "texture/bloodMoon/far.png"};

//    const char * paths[6] = {"texture/test/annotated/right.png", "texture/test/annotated/left.png", "texture/test/annotated/top.png",
//                             "texture/test/annotated/bottom.png", "texture/test/annotated/near.png", "texture/test/annotated/far.png"};

//    const char * paths[6] = {"texture/test/right.png", "texture/test/left.png", "texture/test/top.png",
//     "texture/test/bottom.png", "texture/test/near.png", "texture/test/far.png"};

//    const char * paths[6] = {"texture/graceCathedral/right.png", "texture/graceCathedral/left.png", "texture/graceCathedral/top.png",
//                             "texture/graceCathedral/bottom.png", "texture/graceCathedral/near.png", "texture/graceCathedral/far.png"};

    skybox->setSkybox(paths);
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
    float eyeX = -5.0f;
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

    mProjectionMatrix = new Matrix();
    mCurrentRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix = new Matrix();
    mAccumulatedRotationMatrix->identity();

    mDeltaX = 0.0f;
    mDeltaY = 0.0f;

    //obj->mModelMatrix.rotateLocal(-90, 0, 1, 0);
    //obj->mModelMatrix.rotateLocal(-90, 0, 0, 1);
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
    currentTime = clock();
    //updateFPS(time);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do a compile rotation every 10 seconds;
    long time = GLUtils::currentTimeMillis() % 10000L;
    float angleInDegrees = (360.0f / 100000.0f) * ((int) time);

    // Draw a cube.
    // Translate the cube into the screen.
    //mModelMatrix->identity();
    //mModelMatrix->translate(0.0f, 0.0f, -3.5f);
    //obj->mModelMatrix.rotate(90, 0, 1, 0);
    //obj->mModelMatrix.rotate(90, 1, 0, 0);
    //skybox->mModelMatrix.rotate(1, 0, 0, 1);
    //obj->setRotation(0.0174f, 0.0f, -0.0174f);

    //obj->mModelMatrix.rotateLocal(1, 0, 1, 0);
    //envmap->updateLightCoeff(obj->mModelMatrix);

    // Touch rotation
    mCurrentRotationMatrix->identity();
    mCurrentRotationMatrix->rotate(mDeltaX, 0.0f, 1.0f, 0.0f);
    mCurrentRotationMatrix->rotate(mDeltaY, 1.0f, 0.0f, 0.0f);
    mDeltaX = 0.0f;
    mDeltaY = 0.0f;
    mAccumulatedRotationMatrix->multiply(*mCurrentRotationMatrix, *mAccumulatedRotationMatrix);

    // Set object and skybox V and P
    //obj->mViewMatrix.multiply(*mViewMatrix, *mAccumulatedRotationMatrix);
    obj->mModelMatrix = *mAccumulatedRotationMatrix;
    obj->mViewMatrix = *mViewMatrix;
    obj->mProjectionMatrix = *mProjectionMatrix;
    envmap->updateLightCoeff(obj->mModelMatrix);

    //skybox->mModelMatrix = *mAccumulatedRotationMatrix;
    //envmap->updateLightCoeff(skybox->mModelMatrix);

    //skybox->mViewMatrix.multiply(*mViewMatrix, *mAccumulatedRotationMatrix);
    skybox->mViewMatrix = *mViewMatrix;
    skybox->mProjectionMatrix = *mProjectionMatrix;

    skybox->renderer();
    obj->renderer();

//    float lastDeltaTime = CLOCKS_PER_SEC/deltaTime;
//    deltaTime = clock() - currentTime;
//    float currentDeltaTime = CLOCKS_PER_SEC/(deltaTime);
//    if(abs(int(lastDeltaTime) - int(currentDeltaTime)) > 1)
//        updateFPS(currentDeltaTime);
//    updateFPS(10.0f);

}

void MyDev01::setDelta(float x, float y)
{
    mDeltaX = x;
    mDeltaY = y;
}



