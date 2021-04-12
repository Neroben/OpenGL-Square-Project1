// Дата создания: 2019-05-01
// Автор: Осипов Олег Васильевич
// Copyright © 2019 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем. Все права защищены.

#ifndef JCUBE_H
#define JCUBE_H

#include <QVector3D>
#include "JPolygon.h"

class JCube
{
    ///
    /// \brief Массив из 8 точек параллелепипеда
    ///
    QVector3D V[8];

public:

    // Выделен ли куб мышью?
    bool is_selecting;

    ///
    /// \brief Массив из 6 граней параллелепипеда
    ///
    QVector<JPolygon> polygons;

    JCube();

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
    int intersects(const JRay& ray, QVector3D* R) const;
};

#endif // JCUBE_H