//
// Created by sdont on 13.04.2021.
//

#include <math.h>
#include "JPlane.h"


JPlane::JPlane()
{
    A = B = C = D = 0;
}


// Конструктор плоскости
JPlane::JPlane(float _A, float _B, float _C, float _D) : A(_A), B(_B), C(_C), D(_D)
{
    double R = sqrt(A*A + B*B + C*C);

    A /= R;
    B /= R;
    C /= R;
    D /= R;
}


float JPlane::distanceTo(const QVector3D& V) const
{
    return A * V.x() + B * V.y() + C * V.z() + D;
}


QVector3D JPlane::intersects(const JRay &ray) const
{
    QVector3D a = ray.A(), N = ray.direction();
    double Z = A*N.x() + B*N.y() + C*N.z();
    double dist = A*a.x() + B*a.y() + C*a.z() + D; // Расстояние от точки a(x, y, z) до плоскости
    double t = dist / Z;
    return a - N * t;
}


QVector3D JPlane::normal() const
{
    return QVector3D(A, B, C);
}


JPlane JPlane::operator -() const
{
    return JPlane(-A, -B, -C, -D);
}
