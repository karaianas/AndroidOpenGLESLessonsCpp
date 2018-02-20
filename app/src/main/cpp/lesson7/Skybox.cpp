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

void Skybox::initialize(GLuint mProgramHandle)
{

    // (1) Generate buffers
    GLuint buffers[1];
    glGenBuffers(1, buffers);

    // (2) Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

    glBufferData(GL_ARRAY_BUFFER, positions.size() * BYTES_PER_FLOAT,
                 positions.data(),
                 GL_STATIC_DRAW);

    // IMPORTANT: Unbind from the buffer when we're done with it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mPositionsBufferIdx = buffers[0];

    // near is back(behind me)
//    const char * paths[6] = {"texture/bloodMoon/right.png", "texture/bloodMoon/left.png", "texture/bloodMoon/top.png",
//                             "texture/bloodMoon/bottom.png", "texture/bloodMoon/near.png", "texture/bloodMoon/far.png"};
    const char * paths[6] = {"texture/test/annotated/right.png", "texture/test/annotated/left.png", "texture/test/annotated/top.png",
                             "texture/test/annotated/bottom.png", "texture/test/annotated/near.png", "texture/test/annotated/far.png"};
//    const char * paths[6] = {"texture/test/right.png", "texture/test/left.png", "texture/test/top.png",
//     "texture/test/bottom.png", "texture/test/near.png", "texture/test/far.png"};
//    const char * paths[6] = {"texture/graceCathedral/right.png", "texture/graceCathedral/left.png", "texture/graceCathedral/top.png",
//                             "texture/graceCathedral/bottom.png", "texture/graceCathedral/near.png", "texture/graceCathedral/far.png"};
    mSkyboxBufferIdx = GLUtils::loadCubemap(paths);

    // +++++++++++++++++++++
    //GLUtils::getLightCoeff("texture/right.png");
    // +++++++++++++++++++++

    mModelMatrix = Matrix();
    mModelMatrix.identity();
    mMVPMatrix = Matrix();
    mMVMatrix = Matrix();
    mViewMatrix = Matrix();
    mProjectionMatrix = Matrix();

    program = mProgramHandle;
}

void Skybox::renderer()
{
    //glBindFramebuffer(GL_FRAMEBUFFER, onscreen_framebuffer);

    //glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    mPositionHandle = (GLuint) glGetAttribLocation(program, "a_Position");
    mMVPMatrixHandle = (GLuint) glGetUniformLocation(program, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(program, "u_MVMatrix");

    // Pass in the position information
    glBindBuffer(GL_ARRAY_BUFFER, mPositionsBufferIdx);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, POSITION_STEP, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in the texture information
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyboxBufferIdx);
    glUniform1i(mSkyboxHandle, 0);

    // Pass in matrix information
    mMVMatrix.multiply(mViewMatrix, mModelMatrix);
    glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, mMVMatrix.mData);

    mMVPMatrix.multiply(mProjectionMatrix, mMVMatrix);
    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, mMVPMatrix.mData);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

