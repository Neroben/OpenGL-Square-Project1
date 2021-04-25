//
// Created by sdont on 13.04.2021.
//

#ifndef JCUBE_H
#define JCUBE_H

#include <QVector3D>
#include <QMatrix4x4>
#include "JPolygon.h"

class JCube {
    ///
    /// \brief Массив из 8 точек параллелепипеда
    ///
    QVector3D V[8];

public:

    // Выделен ли куб мышью?
    bool is_selecting;

    // Массивы для шейдеров
    QVector3D vertices[24]; // Массив вершин

    QVector3D normales[24]; // Массив нормалей

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

    QVector3D center;

    JCube(){};

    ~JCube(){};

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

    void updateDepth(QMatrix4x4 projectMatrix);
};


#endif //JCUBE_H
