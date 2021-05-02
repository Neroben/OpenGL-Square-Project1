// Автор: Осипов Олег Васильевич
// Copyright © 2019 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем. Все права защищены.

#include "JSphere.h"
#include <math.h>

JSphere::JSphere() {
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(0, 0, -10);
}

void JSphere::init() {
    buildVerticesSmooth();
}

void JSphere::clearArrays() {
    std::vector<QVector3D>().swap(points);
    std::vector<float>().swap(texCoords);
    std::vector<unsigned int>().swap(lineIndices);
}

void JSphere::buildVerticesSmooth() {
    const float PI = acos(-1);

    // clear memory of prev arrays
    clearArrays();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            addPoints(x, y, z);

            // vertex tex coord between [0, 1]
            s = (float) j / sectorCount;
            t = (float) i / stackCount;
            addTexCoord(s, t);
        }
    }

    unsigned int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if(i != 0) {
                polygons.push_back(JPolygon(&points[k1], &points[k2], &points[k1 + 1]));
            }
            if(i != (stackCount-1)) {
                polygons.push_back(JPolygon(&points[k1 + 1], &points[k2], &points[k2 + 1]));
            }
        }
    }

    for (int i = 0; i < polygons.size(); i++) {
        QVector3D a[3];
        polygons[i].getVertices((float *) &a);
        vertices.push_back(a[0]);
        vertices.push_back(a[1]);
        vertices.push_back(a[2]);

        QVector3D b[3];
        polygons[i].getNormals((float *) &b);
        normals.push_back(b[0]);
        normals.push_back(b[1]);
        normals.push_back(b[2]);

    }
}

void JSphere::addPoints(float x, float y, float z) {
    points.emplace_back(x, y, z);
}

void JSphere::addTexCoord(float s, float t) {
    texCoords.push_back(s);
    texCoords.push_back(t);
}


//Инициализация переменных для вычисление глубины
void JSphere::updateDepth(QMatrix4x4 projectMatrix) {
    // Умножим матрицы проектирования и видовую матрицы
    Q = projectMatrix * modelViewMatrix;

    // Вычислим обратную матрицу
    IQ = Q.inverted();
}