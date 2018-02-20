//
// Created by karaianas on 2/14/2018.
//

#include "Obj.h"
#define LOG_TAG "Obj"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

using namespace std;

int Obj::BYTES_PER_FLOAT = 4;
int Obj::BYTES_PER_SHORT = 2;
int Obj::POSITION_STEP = 3;
int Obj::NORMAL_STEP = 3;
int Obj::COEFF_STEP = 4;

Obj::Obj()
{
}

void Obj::initialize(GLuint mProgramHandle)
{
    // (1) Generate buffers
    GLuint buffers[4];
    glGenBuffers(4, buffers);

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
    glBufferData(GL_ARRAY_BUFFER, coeffs.size() * BYTES_PER_FLOAT, coeffs.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()
                                          * BYTES_PER_SHORT,
                 indices.data(), GL_STATIC_DRAW);

    // IMPORTANT: Unbind from the buffer when we're done with it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mPositionsBufferIdx = buffers[0];
    mNormalsBufferIdx = buffers[1];
    mCoeffsBufferIdx = buffers[2];
    mIndexBufferIdx = buffers[3];

    mModelMatrix = Matrix();
    mModelMatrix.identity();

    //--------- Max Planck's head is rotated, so...
    //mModelMatrix.rotate(-90, 0, 1, 0);
    //mModelMatrix.rotate(-90, 1, 0, 0);
    //--------------------------------------

    mMVPMatrix = Matrix();
    mMVMatrix = Matrix();
    mViewMatrix = Matrix();
    mProjectionMatrix = Matrix();

    program = mProgramHandle;
}

void Obj::renderer()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    mMVPMatrixHandle = (GLuint) glGetUniformLocation(program, "u_MVPMatrix");
    mMVMatrixHandle = (GLuint) glGetUniformLocation(program, "u_MVMatrix");
    mLightHandle = (GLuint) glGetUniformLocation(program, "u_Light");
    mPositionHandle = (GLuint) glGetAttribLocation(program, "a_Position");
    mNormalHandle = (GLuint) glGetAttribLocation(program, "a_Normal");
    mCoeffHandle = (GLuint) glGetAttribLocation(program, "a_Coeff");

    // Pass in the position information
    glBindBuffer(GL_ARRAY_BUFFER, mPositionsBufferIdx);
    glEnableVertexAttribArray(mPositionHandle);
    glVertexAttribPointer(mPositionHandle, POSITION_STEP, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in the normal information
    glBindBuffer(GL_ARRAY_BUFFER, mNormalsBufferIdx);
    glEnableVertexAttribArray(mNormalHandle);
    glVertexAttribPointer(mNormalHandle, NORMAL_STEP, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in the coefficient information
    glBindBuffer(GL_ARRAY_BUFFER, mCoeffsBufferIdx);
    glEnableVertexAttribArray(mCoeffHandle);
    glVertexAttribPointer(mCoeffHandle, COEFF_STEP, GL_FLOAT, GL_FALSE, 0, 0);

    // Pass in matrix information
    mMVMatrix.multiply(mViewMatrix, mModelMatrix);
    glUniformMatrix4fv(mMVMatrixHandle, 1, GL_FALSE, mMVMatrix.mData);

    mMVPMatrix.multiply(mProjectionMatrix, mMVMatrix);
    glUniformMatrix4fv(mMVPMatrixHandle, 1, GL_FALSE, mMVPMatrix.mData);

    // Pass in light coefficient information
    //vector<float> lights = {1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
    //lights = {.79f, .44f, .54f, .39f, .35f, .60f,-.34f, -.18f, -.27f,-.29f, -.06f, .01f};
    // Not halved
    //lights = {4.92f, 2.54f, 2.22f, 0.09f, -0.10f, 0.82f, 3.90f, 0.50f, 1.13f, 0.89f, 0.59f, -0.67f};

    // Halved
    lights = {2.46f, 1.27f, 1.11f, 0.05f, -0.053f, 0.41f, 1.95f, 0.25f, 0.57f, 0.45f, 0.30f, -0.34f};
    // Blood moon
//    lights = {1.446891f, 0.531626f, 0.532157f,  -0.118592f,-0.049557f, 0.001953f, 0.766567f, 0.275998f, 0.639483f, -0.114568f, -0.060870f, -0.022931f};
//    for(int i = 0; i < lights.size(); i++)
//        lights[i] *= 1.5f;
    glUniform3fv(mLightHandle, 4, &lights[0]);

    //glDrawArrays(GL_TRIANGLES, 0, positions.size() / 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferIdx);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Obj::computeLightCoeff()
{

}

vector<float> processLine(string line, int size)
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
            if(result.size() == size)
                return result;
            //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%f", result.back());
        }
    }

   // __android_log_print(ANDROID_LOG_INFO, "MyDev", "%d", result.size());
    return result;
}

vector<short> processLine2(string line, int size)
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
            if(result.size() == size)
                return result;
            //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%f", result.back());
        }
    }

    // __android_log_print(ANDROID_LOG_INFO, "MyDev", "%d", result.size());
    return result;
}

void Obj::parser()
{
    //string path = "/planeNsphere.";
    string path = "/maxPlanck.";
    string modelPath = "models" + path + "model";
    string coeffPath = "coefficients" + path + "coeff";
    const char *buffer = GLUtils::openTextFile(modelPath.c_str());

    int lineCount = 0;
    int vcounter = 0;
    int fcounter = 0;
    // Need to change the limit at some point
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
        vector<float> numbers = processLine(s, 6);
        if(numbers.size() == 6)
        {
            //float len = sqrt(numbers[0]*numbers[0] + numbers[1]*numbers[1] + numbers[2]*numbers[2]);
            float scaleFactor = 0.1f;
            positions.push_back(numbers[0] * scaleFactor);// / len);
            positions.push_back(numbers[1] * scaleFactor);// / len);
            positions.push_back(numbers[2] * scaleFactor);// / len);

            normals.push_back(numbers[3]);
            normals.push_back(numbers[4]);
            normals.push_back(numbers[5]);
            vcounter++;
        }
        else
        {
            vector<short> sth = processLine2(s, 4);
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

    int order = 2;
    int numCoeff = order * order;

    const char *buffer2 = GLUtils::openTextFile(coeffPath.c_str());
    lineCount = 0;
    for(int i = 0; lineCount < positions.size() / 3; i++)
    {
        string s;
        while (buffer2[i] != '\n')
        {
            if(buffer2[i] == '\0')
                break;
            s.push_back(buffer2[i]);
            i++;
        }
        lineCount++;
        vector<float> numbers = processLine(s, numCoeff);
        if(numbers.size() == numCoeff)
        {
            for(int j = 0; j < numCoeff; j++)
                coeffs.push_back(numbers[j]);
        }
        //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%d", numbers.size());
    }
    //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%d", coeffs.size() / 4);
//        char c = buffer[i];
//        string s = to_string(c);
//        const char* a = s.c_str();
//        __android_log_print(ANDROID_LOG_INFO, "MyDev", "%s", a);

    //__android_log_print(ANDROID_LOG_INFO, "MyDev", "%d, %d", indices.size(), fcounter);
}



