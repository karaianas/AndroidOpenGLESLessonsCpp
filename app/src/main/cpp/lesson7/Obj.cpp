//
// Created by karaianas on 2/14/2018.
//

#include <cstdlib>
#include "Obj.h"

#include <ctype.h>

using namespace std;

int Obj::BYTES_PER_FLOAT = 4;
int Obj::BYTES_PER_SHORT = 2;
int Obj::POSITION_STEP = 3;
int Obj::NORMAL_STEP = 3;
//int Obj::TEXCOORD_STEP = 2;
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

vector<float> processLine(string line)
{
    vector<float> result;
    if((line[0] <= 57 && line[0] >= 48) || (line[0] == 45))
    {
        for(int i = 0; i < line.length(); i++)
        {
            string number;
            while(line[i] != ' ')
            {
                number.push_back(line[i]);
                i++;
            }
            result.push_back(stof(number));
            //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%f", result.back());
        }
    }

   // __android_log_print(ANDROID_LOG_INFO, "MyDev", "%d", result.size());
    return result;
}

vector<short> processLine2(string line)
{
    vector<short> result;
    if((line[0] <= 57 && line[0] >= 48) || (line[0] == 45))
    {
        for(int i = 0; i < line.length(); i++)
        {
            string number;
            while(line[i] != ' ')
            {
                number.push_back(line[i]);
                i++;
            }
            result.push_back(short(stoi(number)));
            //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%f", result.back());
        }
    }

    // __android_log_print(ANDROID_LOG_INFO, "MyDev", "%d", result.size());
    return result;
}


void Obj::parser()
{
    const char *buffer = GLUtils::openTextFile("models/planeNsphere.model");
    //const char *buffer = GLUtils::openTextFile("models/maxPlanck.model");

    int lineCount = 0;
    int vcounter = 0;
    int fcounter = 0;
    for(int i = 0; lineCount < 10000; i++) {
        string s;
        while (buffer[i] != '\n')
        {
            if(buffer[i] == '\0')
                break;
            s.push_back(buffer[i]);
            i++;
        }
        lineCount++;
        vector<float> numbers = processLine(s);
        if(numbers.size() == 6)
        {
            positions.push_back(numbers[0]);
            positions.push_back(numbers[1]);
            positions.push_back(numbers[2]);

            normals.push_back(numbers[3]);
            normals.push_back(numbers[4]);
            normals.push_back(numbers[5]);
            vcounter++;
        }
        else
        {
            vector<short> sth = processLine2(s);
            if(sth.size() == 4)
            {
                indices.push_back(sth[1]);
                indices.push_back(sth[2]);
                indices.push_back(sth[3]);
                //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%d", sth.size());
                fcounter++;
            }
        }

        //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%s", s.c_str());
    }

    //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%d", fcounter);
//        char c = buffer[i];
//        string s = to_string(c);
//        const char* a = s.c_str();
//        __android_log_print(ANDROID_LOG_INFO, "MyDev", "%s", a);


    __android_log_print(ANDROID_LOG_INFO, "MyDev", "%d, %d", indices.size(), fcounter);
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

//    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
//    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * BYTES_PER_FLOAT,
//                 texCoords.data(),
//                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()
                                          * BYTES_PER_SHORT,
                 indices.data(), GL_STATIC_DRAW);

    // IMPORTANT: Unbind from the buffer when we're done with it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mPositionsBufferIdx = buffers[0];
    mNormalsBufferIdx = buffers[1];
    //mTexCoordsBufferIdx = buffers[2];
    mIndexBufferIdx = buffers[2];
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
//    glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsBufferIdx);
//    glEnableVertexAttribArray(mTextureCoordHandle);
//    glVertexAttribPointer(mTextureCoordHandle, TEXCOORD_STEP, GL_FLOAT,
//                          GL_FALSE,
//                          0, 0);

    // Clear the currently bound buffer (so future OpenGL calls do not use this buffer).
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Draw the mCubes.
    //glDrawArrays(GL_TRIANGLES, 0, mActualCubeFactor * mActualCubeFactor * mActualCubeFactor * 36);
    //glDrawArrays(GL_TRIANGLES, 0, positions.size() / 3);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferIdx);
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_SHORT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
