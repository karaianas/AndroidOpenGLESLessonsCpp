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