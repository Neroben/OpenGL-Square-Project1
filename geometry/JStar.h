//
// Created by sdont on 04.05.2021.
//

#ifndef OPENGL_JSTAR_H
#define OPENGL_JSTAR_H


#include <QVector3D>
#include <QColor>
#include "JPolygon.h"

class JStar {

    QVector3D *points;

public:
    // Выделена ли звезда мышью?
    bool is_selecting;

    //координаты центра заезды
    QVector3D center;

    float radiusInner = 0.4f;
    float radiusOuter = 1.0f;
    //количество лучей у звезды
    int n = 5;
    //ширина звезды
    float height = 0.5f;

    QVector<JPolygon> polygons;

    QVector<QVector3D> vertices;

    QVector<QVector3D> normals;

    JStar();

    ~JStar();

    void init();

    int intersects(const JRay &ray, QVector3D *R) const;

    void updateDepth(QMatrix4x4 projectMatrix);

    QMatrix4x4 modelViewMatrix;

    // Произведение видовой матрицы и матрицы проектирования
    QMatrix4x4 Q;

    // Матрица, обратная матрице Q
    QMatrix4x4 IQ;
};


#endif //OPENGL_JSTAR_H
