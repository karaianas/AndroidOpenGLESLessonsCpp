//
// Created by karaianas on 2/14/2018.
//

#include "Obj.h"

int Obj::BYTES_PER_FLOAT = 4;
int Obj::POSITION_STEP = 3;
int Obj::NORMAL_STEP = 3;
int Obj::TEXCOORD_STEP = 2;
int Obj::TRANS_STEP = 3;


void Obj::test()
{
    //__android_log_print(ANDROID_LOG_INFO, "MyDev", "hello");


    positions.push_back(0.0f);
    positions.push_back(0.0f);
    positions.push_back(0.0f);

    positions.push_back(1.0f);
    positions.push_back(0.0f);
    positions.push_back(0.0f);

    positions.push_back(1.0f);
    positions.push_back(1.0f);
    positions.push_back(0.0f);

    positions.push_back(0.0f);
    positions.push_back(0.0f);
    positions.push_back(0.0f);

    positions.push_back(1.0f);
    positions.push_back(1.0f);
    positions.push_back(0.0f);

    positions.push_back(0.0f);
    positions.push_back(1.0f);
    positions.push_back(0.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    normals.push_back(0.0f);
    normals.push_back(0.0f);
    normals.push_back(1.0f);

    /*
    texCoords.push_back(0.0f);
    texCoords.push_back(0.0f);

    texCoords.push_back(0.0f);
    texCoords.push_back(1.0f);

    texCoords.push_back(1.0f);
    texCoords.push_back(0.0f);

    texCoords.push_back(0.0f);
    texCoords.push_back(1.0f);

    texCoords.push_back(1.0f);
    texCoords.push_back(1.0f);

    texCoords.push_back(1.0f);
    texCoords.push_back(0.0f);
     */

}

void Obj::initialize()
{
    // (1) Generate buffers
    GLuint buffers[3];
    glGenBuffers(3, buffers);

    // (2) Bind buffers
    // Bind to the buffer. Future commands will affect this buffer specifically.
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

    glBufferData(GL_ARRAY_BUFFER, positions.size() * BYTES_PER_FLOAT,
                 positions.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * BYTES_PER_FLOAT, normals.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * BYTES_PER_FLOAT,
                 texCoords.data(),
                 GL_STATIC_DRAW);

    // IMPORTANT: Unbind from the buffer when we're done with it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mPositionsBufferIdx = buffers[0];
    mNormalsBufferIdx = buffers[1];
    mTexCoordsBufferIdx = buffers[2];
}

void Obj::renderer()
{
    // Pass in the position information
    glBindBuffer(GL_ARRAY_BUFFER, mPositionsBufferIdx);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, POSITION_STEP, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in the normal information
    glBindBuffer(GL_ARRAY_BUFFER, mNormalsBufferIdx);
    glEnableVertexAttribArray(mNormalHandle);
    glVertexAttribPointer(mNormalHandle, NORMAL_STEP, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in the texture information
    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsBufferIdx);
    glEnableVertexAttribArray(mTextureCoordHandle);
    glVertexAttribPointer(mTextureCoordHandle, TEXCOORD_STEP, GL_FLOAT,
                          GL_FALSE,
                          0, 0);

    // Clear the currently bound buffer (so future OpenGL calls do not use this buffer).
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Draw the mCubes.
    //glDrawArrays(GL_TRIANGLES, 0, mActualCubeFactor * mActualCubeFactor * mActualCubeFactor * 36);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
