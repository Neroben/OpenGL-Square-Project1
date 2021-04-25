//
// Created by sdont on 13.04.2021.
//

#ifndef OPENGL_JCUBE_H
#define OPENGL_JCUBE_H

#include <QVector3D>
#include "JPolygon.h"

class JCube {
    ///
    /// \brief Массив из 8 точек параллелепипеда
    ///
    QVector3D V[8];

public:

    // Выделен ли куб мышью?
    bool is_selecting;

    QVector<QVector3D> vertices;
    QVector<QVector3D> normales;

    ///
    /// \brief Массив из 6 граней параллелепипеда
    ///
    QVector<JPolygon> polygons;

    // Матрица видового преобразования
    QMatrix4x4 modelViewMatrix;

    // Произведение видовой матрицы и матрицы проектирования
    QMatrix4x4 Q;

    // Матрица, обратная матрице Q
    QMatrix4x4 IQ;

    float translateX = 0;

    float translateY = 0;

    float translateZ = 0;

    JCube(){};

    ~JCube();

    ///
    /// \brief init
    /// \param A - координаты первой вершины
    /// \param B - координаты противоположной вершины
    /// Создаёт куб, грани которого параллельны координатным плоскостям
    void init(QVector3D A, QVector3D B);


    ///
    /// \brief intersects
    /// \param ray - параметры луча в трёхмерном пространстве
    /// \return Возвращает количество точек пересечения луча с кубом
    ///
    int intersects(const JRay &ray, QVector3D *R) const;

    void translate(float x, float y, float z);

    void updatePoints();

    void rotate(float dx, float dy);

    void scale(float x, float y, float z);

    void initDepth(QMatrix4x4 projectMatrix);

    void resetModelView();
};


#endif //OPENGL_JCUBE_H
