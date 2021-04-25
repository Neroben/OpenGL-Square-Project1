//
// Created by sdont on 13.04.2021.
//

#include "JCube.h"

void JCube::init(QVector3D A, QVector3D B) {
    // Задаём координаты 8 вершин
    V[0] = QVector3D(A.x(), A.y(), A.z());
    V[1] = QVector3D(A.x(), A.y(), B.z());
    V[2] = QVector3D(A.x(), B.y(), A.z());
    V[3] = QVector3D(A.x(), B.y(), B.z());
    V[4] = QVector3D(B.x(), A.y(), A.z());
    V[5] = QVector3D(B.x(), A.y(), B.z());
    V[6] = QVector3D(B.x(), B.y(), A.z());
    V[7] = QVector3D(B.x(), B.y(), B.z());

    // 6 граней куба
    polygons.push_back(JPolygon(&V[6], &V[7], &V[5], &V[4]));
    polygons.push_back(JPolygon(&V[0], &V[1], &V[3], &V[2]));
    polygons.push_back(JPolygon(&V[4], &V[5], &V[1], &V[0]));
    polygons.push_back(JPolygon(&V[2], &V[3], &V[7], &V[6]));
    polygons.push_back(JPolygon(&V[0], &V[2], &V[6], &V[4]));
    polygons.push_back(JPolygon(&V[5], &V[7], &V[3], &V[1]));

    is_selecting = false; // Куб не выделен

    // Инициализация массива вершин и нормалей
    for (int i = 0; i < polygons.size(); i++)
    {
        polygons[i].getVertices((float*)(vertices + i*4));
        polygons[i].getNormals((float*)(normales + i*4));
    }
}


int JCube::intersects(const JRay &ray, QVector3D *R) const {
    QVector3D C;
    int k = 0;
    for (int i = 0; i < polygons.size(); i++) {
        // Если луч пересекает хотя бы одну грань, то считаем, что луч пересёк куб
        // В R записываем координаты точек пересечения с гранями
        if (polygons[i].intersects(ray, C))
            R[k++] = C;
    }
    return k;
}

//Инициализация переменных для вычисление глубины
void JCube::updateDepth(QMatrix4x4 projectMatrix) {
    // Умножим матрицы проектирования и видовую матрицы
    Q = projectMatrix * modelViewMatrix;

    // Вычислим обратную матрицу
    IQ = Q.inverted();
}