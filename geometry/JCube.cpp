//
// Created by sdont on 13.04.2021.
//

#include "JCube.h"

JCube::JCube()
{

}

void JCube::init(QVector3D A, QVector3D B)
{
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
}


int JCube::intersects(const JRay &ray, QVector3D* R) const
{
    QVector3D C;
    int k = 0;
    for (int i=0; i < polygons.size(); i++)
    {
        // Если луч пересекает хотя бы одну грань, то считаем, что луч пересёк куб
        // В R записываем координаты точек пересечения с гранями
        if (polygons[i].intersects(ray, C))
            R[k++] = C;;
    }
    return k;
}

float* JCube::getVertices() {
    float *vertices = new float[6*4*3];

    for(int i = 0; i < polygons.size(); i++) {
        JPolygon p = polygons[i];
        p.getVertices(&(vertices[i*4*3]));
    }
    return vertices;
}

float* JCube::getNormales() {
    float *normales = new float[6*4*3];

    for(int i = 0; i < polygons.size(); i++) {
        JPolygon p = polygons[i];
        p.getNormals(&(normales[i*4*3]));
    }
    return normales;
}