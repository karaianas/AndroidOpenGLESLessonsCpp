//
// Created by karaianas on 2/18/2018.
//

#include "EnvMap.h"

#define LOG_TAG "EnvMap"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

using namespace std;

vector<float> processMap(string line, int size);

vector<float> EnvMap::getLightCoeff(int order)
{
    // Calculate 4 light coeff for now
    int num = 4;
    return vector<float, allocator<float>>();
}

vector<GLubyte> EnvMap::readPixel(int i, int j)
{
    int offset = 4 * (i * width + j);
    vector<GLubyte> rgb;
    rgb.push_back(pixels[offset]);
    rgb.push_back(pixels[offset + 1]);
    rgb.push_back(pixels[offset + 2]);
    //LOGD("%d %d %d", rgb[0], rgb[1], rgb[2]);
    return rgb;
}

void EnvMap::renderToTexture()
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //Create the texture
    GLuint temp = GLUtils::loadTexture("texture/test/equirectangular_320160.png");

    //Bind the texture to your FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, temp, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Read from frame buffer
    glViewport(0, 0, width, height);
    pixels = (GLubyte*) malloc(width * height * sizeof(GLubyte) * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

//    for(int i = 0; i < 12; i++)
//        LOGD("%d", pixels[i]);

    // Delete framebuffer
    glDeleteFramebuffers(1, &FBO);
}

vector<float> EnvMap::getPixel(int i, int j)
{
    //LOGD("%f %f %f", colors[i * w + j][0], colors[i * w + j][1], colors[i * w + j][2]);
    return colors[i * width + j];
}

void EnvMap::parser()
{
    string path = "models/image_output.txt";
    const char *buffer = GLUtils::openTextFile(path.c_str());
    width = 320;
    height = 160;

    // Need to change the limit at some point
    int lineCount = 0;
    for(int i = 0; lineCount < width * height; i++)
    {
        string s;
        while (buffer[i] != '\n') {
            if (buffer[i] == '\0')
                break;
            s.push_back(buffer[i]);
            i++;
        }
        vector<float> numbers = processMap(s, 3);
        colors.push_back(numbers);
        lineCount++;
    }

    LOGD("%d", (int)colors.size());

}

vector<float> processMap(string line, int size)
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