//
// Created by karaianas on 2/17/2018.
//

#include "Skybox.h"

#define LOG_TAG "Sky"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

int BYTES_PER_FLOAT = 4;
int POSITION_STEP = 3;


Skybox::Skybox()
{
    for(int i = 0; i < positions.size(); i++)
        positions[i] *= 10.f;
}

void Skybox::initialize(GLuint program)
{

    // (1) Generate buffers
    GLuint buffers[1];
    glGenBuffers(1, buffers);

    // (2) Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

    glBufferData(GL_ARRAY_BUFFER, positions.size() * BYTES_PER_FLOAT,
                 positions.data(),
                 GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);


    // IMPORTANT: Unbind from the buffer when we're done with it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mPositionsBufferIdx = buffers[0];

    const char * paths[6] = {"texture/right.png", "texture/left.png", "texture/top.png", "texture/bottom.png", "texture/near.png", "texture/far.png"};
    mSkyboxBufferIdx = GLUtils::loadCubemap(paths);

//    mPositionHandle = (GLuint) glGetUniformLocation(program, "a_Position");
//    mSkyboxHandle = (GLuint) glGetUniformLocation(program, "skybox");
//    mMVPMatrixHandle = (GLuint) glGetUniformLocation(program, "u_MVPMatrix");
//    mMVMatrixHandle = (GLuint) glGetUniformLocation(program, "u_MVMatrix");

    mModelMatrix = new Matrix();
    mModelMatrix->identity();
    mMVPMatrix = new Matrix();
    mMVMatrix = new Matrix();
    mViewMatrix = new Matrix();
    mProjectionMatrix = new Matrix();
}

void Skybox::renderer(GLuint program)
{
    //LOGD("%d", positions.size()/3);
    //LOGD("%d %d %d", program, mMVMatrixHandle, mMVPMatrixHandle);

    // Pass in the position information
    glBindBuffer(GL_ARRAY_BUFFER, mPositionsBufferIdx);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, POSITION_STEP, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in the texture information
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxBufferIdx);
    glUniform1i(mSkyboxHandle, 0);

    // Pass in matrix information
    //mMVPMatrix->multiply(*mViewMatrix, *mModelMatrix);
    glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, mMVPMatrix->mData);

    //mMVPMatrix->multiply(*mProjectionMatrix, *mMVPMatrix);
    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, mMVPMatrix->mData);

    //glDrawArrays(GL_POINTS, 0, 6);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::setViewMatrix(Matrix *V)
{
    mViewMatrix = V;
}

void Skybox::setProjectionMatrix(Matrix *P)
{
    mProjectionMatrix = P;
}

