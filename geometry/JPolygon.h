//
// Created by sdont on 13.04.2021.
//

#ifndef JPOLYGON_H
#define JPOLYGON_H

#include <QVector3D>
#include <QVector>
#include "JRay.h"
#include "JPlane.h"

///
/// \brief Класс JPolygon - многоугольник
///
class JPolygon : QVector<QVector3D*> {

    // Уравнение плоскости, на которой лежит многоугольник
    JPlane plane;

    // Процедура вычисляет координаты вектора нормали по первым трём точкам
    void initNormal();

    ///
    /// \brief inside
    /// \param A
    /// \return Возвращает истину, если точка A находится внутри многоугольника
    ///
    bool inside(const QVector3D& R) const;

    bool isSelected = false;

public:
    JPolygon();

    // Конструктор треугольника
    JPolygon(QVector3D* V1, QVector3D* V2, QVector3D* V3);

    // Конструктор выпуклого четырёхугольника
    JPolygon(QVector3D* V1, QVector3D* V2, QVector3D* V3, QVector3D* V4);

    ///
    /// \brief sort_vertices
    /// Сортирует массив вершин по часовой стрелке
    ///
    void sort_vertices();

    ///
    /// \brief intersects
    /// \param ray - параметры луча в трёхмерном пространстве
    /// \return Возвращает истину, если луч ray пересекает многоугольник, или ложь, если не пересекает
    ///
    bool intersects(const JRay& ray, QVector3D& R) const;

    ///
    /// \brief nVertices
    /// \return Возвращает количество вершин многоугольника
    ///
    int nVertices() const;

    ///
    /// \brief operator []
    /// \param index
    /// \return Возвращает координаты вершины многоугольника с номером index
    ///
    QVector3D operator[](int index) const;

    ///
    /// \brief normal
    /// \return Возвращает единичный вектор нормали к плоскости многоугольника
    ///
    QVector3D normal() const;

    void getVertices(float* vertices) const;

    void getNormals(float* normalArr) const;

};


#endif //JPOLYGON_H
