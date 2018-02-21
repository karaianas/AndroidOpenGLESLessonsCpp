//
// Created by karaianas on 2/18/2018.
//

#include "EnvMap.h"

#define LOG_TAG "EnvMap"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

#define PI 3.14159f

using namespace std;

float sinc(float x)
{
    if (fabs(x) < 1.0e-4) return 1.0;
    else return(sin(x) / x);
}

vector<vector<float>>* EnvMap::getLightCoeff(int order)
{
    // Calculate 4 light coeff for now(order of 2)
    int num = order * order;

//    colors->clear();
    delete colors;
    colors = new vector<vector<float>>();

    for(int col = 0; col < num; col++)
    {
        vector<float> temp;
        temp.push_back(0.0f);
        temp.push_back(0.0f);
        temp.push_back(0.0f);
        colors->push_back(temp);
    }

//    float mintheta = 0.0f;
//    float minphi = 0.0f;
//    float maxtheta = 0.0f;
//    float maxphi = 0.0f;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {

            /* We now find the cartesian components for the point (i,j) */
            float theta, phi, x, y, z, domega;

            theta = float(i)/float(height) * PI;
            phi = float(j)/float(width) * 2 * PI;

//            if(phi > maxphi)
//                maxphi = phi;
//            else if(phi < minphi)
//                minphi = phi;
//
//            if(theta > maxtheta)
//                maxtheta = theta;
//            else if(theta < mintheta)
//                mintheta = theta;

            x = sin(theta)*cos(phi);
            y = sin(theta)*sin(phi);
            z = cos(theta);

            float norm = sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));

            domega = (2 * PI / width)*(PI / height)*sinc(theta);

            updateCoeff(i, j, x/norm, y/norm, z/norm, domega);
        }

//    LOGD("%f %f %f", colors[0][0], colors[0][1], colors[0][2]);
//    LOGD("%f %f %f", colors[1][0], colors[1][1], colors[1][2]);
//    LOGD("%f %f %f", colors[2][0], colors[2][1], colors[2][2]);
//    LOGD("%f %f %f", colors[3][0], colors[3][1], colors[3][2]);
//    LOGD("%f %f %f %f", minphi, maxphi, mintheta, maxtheta);

    return colors;
}

void EnvMap::updateCoeff(int i, int j, float x, float y, float z, float domega)
{
    // This is parallelizable
    vector<GLubyte> pixel = readPixel(i, j);
    for (int col = 0; col < 3; col++)
    {
        float c; /* A different constant for each coefficient */

        /* L_{00}.  Note that Y_{00} = 0.282095 */
        c = 0.282095;
        colors->at(0)[col] += float(int(pixel[col]))/255.0f * c*domega;

        /* L_{1m}. -1 <= m <= 1.  The linear terms */
        c = 0.488603;
        colors->at(1)[col] += float(int(pixel[col]))/255.0f * (c*y)*domega;   /* Y_{1-1} = 0.488603 y  */
        colors->at(2)[col] += float(int(pixel[col]))/255.0f * (c*z)*domega;   /* Y_{10}  = 0.488603 z  */
        colors->at(3)[col] += float(int(pixel[col]))/255.0f * (c*x)*domega;   /* Y_{11}  = 0.488603 x  */

//        /* The Quadratic terms, L_{2m} -2 <= m <= 2 */
//
//        /* First, L_{2-2}, L_{2-1}, L_{21} corresponding to xy,yz,xz */
//        c = 1.092548;
//        coeffs[4][col] += hdr[col] * (c*x*y)*domega; /* Y_{2-2} = 1.092548 xy */
//        coeffs[5][col] += hdr[col] * (c*y*z)*domega; /* Y_{2-1} = 1.092548 yz */
//        coeffs[7][col] += hdr[col] * (c*x*z)*domega; /* Y_{21}  = 1.092548 xz */
//
//        /* L_{20}.  Note that Y_{20} = 0.315392 (3z^2 - 1) */
//        c = 0.315392;
//        coeffs[6][col] += hdr[col] * (c*(3 * z*z - 1))*domega;
//
//        /* L_{22}.  Note that Y_{22} = 0.546274 (x^2 - y^2) */
//        c = 0.546274;
//        coeffs[8][col] += hdr[col] * (c*(x*x - y*y))*domega;

    }
}


vector<GLubyte> EnvMap::readPixel(int i, int j)
{
    int offset = 4 * (i * width + j);
    vector<GLubyte> rgb;
    rgb.push_back(pixels[offset]);
    rgb.push_back(pixels[offset + 1]);
    rgb.push_back(pixels[offset + 2]);
    //LOGD("%d %d %d", rgb[0], rgb[1], rgb[2]);
    //LOGD("%f %f %f", float(int(rgb[0]))/255.0f, float(int(rgb[1]))/255.0f, float(int(rgb[2]))/255.0f);
    return rgb;
}

void EnvMap::renderToTexture(const char* path)
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //Create the texture
    GLuint textureID = GLUtils::loadTexture(path);
    //GLuint temp = GLUtils::loadTexture("texture/bloodMoon/equirectangular_512256.png");
//    GLuint temp = GLUtils::loadTexture("texture/test/equirectangular3_320160.png");
    //GLuint temp = GLUtils::loadTexture("texture/graceCathedral/equirectangular_512256.png");
    //GLuint temp = GLUtils::loadTexture("texture/test/equirectangular_320160.png");

    //Bind the texture to your FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Read from frame buffer
    glViewport(0, 0, width, height);
    pixels = (GLubyte*) malloc(width * height * sizeof(GLubyte) * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // Delete framebuffer
    glDeleteFramebuffers(1, &FBO);
}