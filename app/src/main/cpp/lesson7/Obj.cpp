//
// Created by karaianas on 2/14/2018.
//

#include "Obj.h"
#define BYTES_PER_FLOAT 4

void Obj::test()
{
    __android_log_print(ANDROID_LOG_INFO, "MyDev", "hello");
}

void Obj::initialize(vector<float> *positions, vector<float> *normals, vector<float> *textureCoords)
{
   // (1) Generate buffers
    GLuint buffers[3];
    glGenBuffers(3, buffers);

    // Bind to the buffer. Future commands will affect this buffer specifically.
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    // Transfer data from client memory to the buffer.
    // We can release the client memory after this call.
    glBufferData(GL_ARRAY_BUFFER, positions->size() * BYTES_PER_FLOAT,
                 positions->data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, normals->size() * BYTES_PER_FLOAT, normals->data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, textureCoords->size() * BYTES_PER_FLOAT,
                 textureCoords->data(),
                 GL_STATIC_DRAW);

    // IMPORTANT: Unbind from the buffer when we're done with it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mPositionsBufferIdx = buffers[0];
    mNormalsBufferIdx = buffers[1];
    mTexCoordsBufferIdx = buffers[2];
}
