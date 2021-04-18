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

    float *vertices = nullptr;
    float *normales = nullptr;
    float *colors = nullptr;

    ///
    /// \brief Массив из 6 граней параллелепипеда
    ///
    QVector<JPolygon> polygons;

    float *getVertices();

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

    float *getNormales();

    float *getColors();
};


#endif //OPENGL_JCUBE_H
