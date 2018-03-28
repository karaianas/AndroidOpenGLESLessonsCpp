/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

#include "Matrix.h"
#include <string.h>
#include <cmath>

#include <android/log.h>

#define LOG_TAG "Lesson"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)


Matrix::Matrix() {
    identity();
}

Matrix::Matrix(const Matrix &src) {
    loadWith(src);
}

void Matrix::setValue(int i, int j, float value) {

}

void Matrix::print(const char *label) {
    LOGI("%c", *label);
    for (int i = 0; i < 4; i++) {
        const float *d = &(mData[i * 4]);
        LOGI("%f %f %f %f\n", d[0], d[1], d[2], d[3]);
    }
}

bool Matrix::equals(const Matrix &src) {
    bool equals = true;
    const float *d = src.mData;
    for (int i = 0; i < MATRIX_SIZE && equals; i++) {
        if (mData[i] != d[i]) {
            equals = false;
        }
    }
    return equals;
}

void Matrix::loadWith(const Matrix &src) {
    memcpy(mData, src.mData, MATRIX_SIZE * sizeof(float));
}

void Matrix::identity() {
    mData[0] = 1.0f;
    mData[1] = 0.0f;
    mData[2] = 0.0f;
    mData[3] = 0.0f;

    mData[4] = 0.0f;
    mData[5] = 1.0f;
    mData[6] = 0.0f;
    mData[7] = 0.0f;

    mData[8] = 0.0f;
    mData[9] = 0.0f;
    mData[10] = 1.0f;
    mData[11] = 0.0f;

    mData[12] = 0.0f;
    mData[13] = 0.0f;
    mData[14] = 0.0f;
    mData[15] = 1.0f;
}

void Matrix::translate(float x, float y, float z) {
    Matrix *m = newTranslate(x, y, z);
    Matrix *temp = new Matrix(*this);
    if (m != NULL && temp != NULL) {
        multiply(*temp, *m);
    }
    delete m;
    delete temp;
}

void Matrix::scale(float x, float y, float z) {
    Matrix *m = newScale(x, y, z);
    Matrix *temp = new Matrix(*this);
    if (m != NULL && temp != NULL) {
        multiply(*temp, *m);
    }
    delete m;
    delete temp;
}

void Matrix::rotate(float radians, float x, float y, float z) {
    Matrix *m = newRotate(radians, x, y, z);
    Matrix *temp = new Matrix(*this);
    if (m != NULL && temp != NULL) {
        multiply(*temp, *m);
    }
    delete m;
    delete temp;
}

void Matrix::rotateLocal(float radians, float x, float y, float z) {
    Matrix *m = newRotate(radians, x, y, z);
    Matrix *temp = new Matrix(*this);
    if (m != NULL && temp != NULL) {
        multiply(*m, *temp);
    }
    delete m;
    delete temp;
}

void Matrix::transpose(Matrix& result, Matrix& refer)
{
    float m0 = refer.mData[0];
    float m1 = refer.mData[1];
    float m2 = refer.mData[2];
    float m3 = refer.mData[3];

    float m4 = refer.mData[4];
    float m5 = refer.mData[5];
    float m6 = refer.mData[6];
    float m7 = refer.mData[7];

    float m8 = refer.mData[8];
    float m9 = refer.mData[9];
    float m10 = refer.mData[10];
    float m11 = refer.mData[11];

    float m12 = refer.mData[12];
    float m13 = refer.mData[13];
    float m14 = refer.mData[14];
    float m15 = refer.mData[15];

    result.identity();
    result.mData[0] = m0;
    result.mData[1] = m4;
    result.mData[2] = m8;
    result.mData[3] = m12;
    result.mData[4] = m1;
    result.mData[5] = m5;
    result.mData[6] = m9;
    result.mData[7] = m13;
    result.mData[8] = m2;
    result.mData[9] = m6;
    result.mData[10] = m10;
    result.mData[11] = m14;
    result.mData[12] = m3;
    result.mData[13] = m7;
    result.mData[14] = m11;
    result.mData[15] = m15;

}

void Matrix::multiply(const Matrix &l, const Matrix &r) {
    float const *const lhs = l.mData;
    float const *const rhs = r.mData;
    for (int i = 0; i < 4; i++) {
        const int i4 = i * 4;
        float x = 0;
        float y = 0;
        float z = 0;
        float w = 0;

        for (int j = 0; j < 4; j++) {
            const int j4 = j * 4;
            const float e = rhs[i4 + j];
            x += lhs[j4 + 0] * e;
            y += lhs[j4 + 1] * e;
            z += lhs[j4 + 2] * e;
            w += lhs[j4 + 3] * e;
        }

        mData[i4 + 0] = x;
        mData[i4 + 1] = y;
        mData[i4 + 2] = z;
        mData[i4 + 3] = w;
    }
}

Matrix *Matrix::newLookAt(float eyeX, float eyeY, float eyeZ, float centerX,
                          float centerY, float centerZ, float upX, float upY, float upZ) {
    Matrix *m = new Matrix();
    if (m != NULL) {
        // See the OpenGL GLUT documentation for gluLookAt for a description
        // of the algorithm. We implement it in a straightforward way:

        float fx = centerX - eyeX;
        float fy = centerY - eyeY;
        float fz = centerZ - eyeZ;

        // Normalize f
        float rlf = 1.0f / (float) sqrt(fx * fx + fy * fy + fz * fz);
        fx *= rlf;
        fy *= rlf;
        fz *= rlf;

        // compute s = f x up (x means "cross product")
        float sx = fy * upZ - fz * upY;
        float sy = fz * upX - fx * upZ;
        float sz = fx * upY - fy * upX;

        // and normalize s
        float rls = 1.0f / (float) sqrt(sx * sx + sy * sy + sz * sz);
        sx *= rls;
        sy *= rls;
        sz *= rls;

        // compute u = s x f
        float ux = sy * fz - sz * fy;
        float uy = sz * fx - sx * fz;
        float uz = sx * fy - sy * fx;

        float *d = m->mData;
        d[0] = sx;
        d[1] = ux;
        d[2] = -fx;
        d[3] = 0.0f;

        d[4] = sy;
        d[5] = uy;
        d[6] = -fy;
        d[7] = 0.0f;

        d[8] = sz;
        d[9] = uz;
        d[10] = -fz;
        d[11] = 0.0f;

        d[12] = 0.0f;
        d[13] = 0.0f;
        d[14] = 0.0f;
        d[15] = 1.0f;

        m->translate(-eyeX, -eyeY, -eyeZ);
    }
    return m;
}

Matrix *Matrix::newFrustum(float left, float right, float bottom, float top,
                           float near, float far) {
    const float r_width = 1.0f / (right - left);
    const float r_height = 1.0f / (top - bottom);
    const float r_depth = 1.0f / (near - far);
    const float x = 2.0f * (near * r_width);
    const float y = 2.0f * (near * r_height);
    const float A = (right + left) * r_width;
    const float B = (top + bottom) * r_height;
    const float C = (far + near) * r_depth;
    const float D = 2.0f * (far * near * r_depth);
    Matrix *m = new Matrix();
    if (m != NULL) {
        float *d = m->mData;
        d[0] = x;
        d[5] = y;
        d[8] = A;
        d[9] = B;
        d[10] = C;
        d[14] = D;
        d[11] = -1.0f;
        d[1] = 0.0f;
        d[2] = 0.0f;
        d[3] = 0.0f;
        d[4] = 0.0f;
        d[6] = 0.0f;
        d[7] = 0.0f;
        d[12] = 0.0f;
        d[13] = 0.0f;
        d[15] = 0.0f;
    }
    return m;
}

Matrix *Matrix::newTranslate(float x, float y, float z) {
    Matrix *m = new Matrix();
    if (m != NULL) {
        float *d = m->mData;
        d[12] = x;
        d[13] = y;
        d[14] = z;
    }
    return m;
}

Matrix *Matrix::newScale(float x, float y, float z) {
    Matrix *m = new Matrix();
    if (m != NULL) {
        float *d = m->mData;
        d[0] = x;
        d[5] = y;
        d[10] = z;
    }
    return m;
}

Matrix *Matrix::newRotate(float radians, float x, float y, float z) {
    radians *= (float) (3.14159265358979323846 / 180.0f);
    Matrix *m = new Matrix();
    if (m != NULL) {
        float *d = m->mData;
        d[3] = 0;
        d[7] = 0;
        d[11] = 0;
        d[12] = 0;
        d[13] = 0;
        d[14] = 0;
        d[15] = 1;
        float s = (float) sinf(radians);
        float c = (float) cosf(radians);
        if (1.0f == x && 0.0f == y && 0.0f == z) {
            d[5] = c;
            d[10] = c;
            d[6] = s;
            d[9] = -s;
            d[1] = 0;
            d[2] = 0;
            d[4] = 0;
            d[8] = 0;
            d[0] = 1;
        } else if (0.0f == x && 1.0f == y && 0.0f == z) {
            d[0] = c;
            d[10] = c;
            d[8] = s;
            d[2] = -s;
            d[1] = 0;
            d[4] = 0;
            d[6] = 0;
            d[9] = 0;
            d[5] = 1;
        } else if (0.0f == x && 0.0f == y && 1.0f == z) {
            d[0] = c;
            d[5] = c;
            d[1] = s;
            d[4] = -s;
            d[2] = 0;
            d[6] = 0;
            d[8] = 0;
            d[9] = 0;
            d[10] = 1;
        } else {
            float len = sqrt((x * x) + (y * y) + (z * z));
            if (1.0f != len) {
                float recipLen = 1.0f / len;
                x *= recipLen;
                y *= recipLen;
                z *= recipLen;
            }
            float nc = 1.0f - c;
            float xy = x * y;
            float yz = y * z;
            float zx = z * x;
            float xs = x * s;
            float ys = y * s;
            float zs = z * s;
            d[0] = x * x * nc + c;
            d[4] = xy * nc - zs;
            d[8] = zx * nc + ys;
            d[1] = xy * nc + zs;
            d[5] = y * y * nc + c;
            d[9] = yz * nc - xs;
            d[2] = zx * nc - ys;
            d[6] = yz * nc + xs;
            d[10] = z * z * nc + c;
        }
    }
    return m;
}

void Matrix::multiplyVector(float *result, const Matrix &lhs,
                            const float *rhs) {
    const float *d = lhs.mData;
    const float x = rhs[0];
    const float y = rhs[1];
    const float z = rhs[2];
    const float w = rhs[3];
    for (int i = 0; i < 4; i++) {
        const int j = i * 4;
        result[i] = d[j + 0] * x + d[j + 1] * y + d[j + 2] * z + d[j + 3] * w;
    }
}

void mx4transform(float x, float y, float z, float w, const float *pM, float *pDest) {
    pDest[0] = pM[0 + 4 * 0] * x + pM[0 + 4 * 1] * y + pM[0 + 4 * 2] * z + pM[0 + 4 * 3] * w;
    pDest[1] = pM[1 + 4 * 0] * x + pM[1 + 4 * 1] * y + pM[1 + 4 * 2] * z + pM[1 + 4 * 3] * w;
    pDest[2] = pM[2 + 4 * 0] * x + pM[2 + 4 * 1] * y + pM[2 + 4 * 2] * z + pM[2 + 4 * 3] * w;
    pDest[3] = pM[3 + 4 * 0] * x + pM[3 + 4 * 1] * y + pM[3 + 4 * 2] * z + pM[3 + 4 * 3] * w;
}

void Matrix::multiplyMV(float *r, const float *lhs, const float *rhs) {
    mx4transform(rhs[0], rhs[1], rhs[2], rhs[3], lhs, r);
}

float Matrix::length(float x, float y, float z) {
    return (float) sqrt(x * x + y * y + z * z);
}


