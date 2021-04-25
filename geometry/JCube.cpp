//
// Created by sdont on 13.04.2021.
//

#include <QMatrix4x4>
#include "JCube.h"

JCube::~JCube() {
}

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
    updatePoints();

    modelViewMatrix.setToIdentity();
}

void JCube::updatePoints() {
    vertices.clear();
    normales.clear();
    for (int i = 0; i < polygons.size(); i++) {
        JPolygon p = polygons[i];
        for (int j = 0; j < p.nVertices(); j++) {
            vertices.push_back(p[j]);
        }
        for (int j = 0; j < p.nVertices(); j++) {
            normales.push_back(p.normal());
        }
    }
}

int JCube::intersects(const JRay &ray, QVector3D *R) const {
    QVector3D C;
    int k = 0;
    for (int i = 0; i < polygons.size(); i++) {
        // Если луч пересекает хотя бы одну грань, то считаем, что луч пересёк куб
        // В R записываем координаты точек пересечения с гранями
        if (polygons[i].intersects(ray, C))
            R[k++] = C;;
    }
    return k;
}

void JCube::translate(float x, float y, float z) {
    modelViewMatrix.translate(x, y, z);
}

void JCube::rotate(float dx, float dy) {
    // Матрица поворота
    QMatrix4x4 rotateMatrix; // Изначально матрица поворота равна единичной матрице
    rotateMatrix.rotate(-dx, 1, 0);
    rotateMatrix.rotate(-dy, 0, 1);

    modelViewMatrix *= rotateMatrix.transposed();
}

void JCube::scale(float x, float y, float z) {
    modelViewMatrix.scale(x, y, z);
}

void JCube::resetModelView() {
    modelViewMatrix.setToIdentity();
}

//Инициализация переменных для вычисление глубины
void JCube::initDepth(QMatrix4x4 projectMatrix) {
    // Умножим матрицы проектирования и видовую матрицы
    Q = projectMatrix * modelViewMatrix;

    // Вычислим обратную матрицу
    IQ = Q.inverted();
}