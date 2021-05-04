// Автор: Осипов Олег Васильевич
// Copyright © 2019 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем. Все права защищены.

#ifndef OPENGL_JSPHERE_H
#define OPENGL_JSPHERE_H


#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>
#include "JRay.h"
#include "JPlane.h"
#include "JPolygon.h"

class JSphere {


public:
    // Координаты центра сферы
    QVector3D center;

    // Радиус сферы
    float radius = 1;
    int sectorCount = 25;   //секторов на круге xy
    int stackCount = 25;    //секторов на круге xz

    // Цвет сферы
    QColor color;

    QVector<JPolygon> polygons;

    QVector<QVector3D> vertices;

    QVector<QVector3D> normals;

    QVector<QVector3D> points;


    // Флаг, определяющий, выделена ли сфера мышкой
    bool is_selecting;

    QMatrix4x4 modelViewMatrix;

    // Произведение видовой матрицы и матрицы проектирования
    QMatrix4x4 Q;

    // Матрица, обратная матрице Q
    QMatrix4x4 IQ;

    JSphere();


    void updateDepth(QMatrix4x4 projectMatrix);

    void addPoints(float x, float y, float z);

    void init();

    void buildVerticesSmooth();

    int intersects(const JRay &ray, QVector3D *R) const;
};


#endif //OPENGL_JSPHERE_H