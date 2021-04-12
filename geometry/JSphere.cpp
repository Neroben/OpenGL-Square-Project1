// Автор: Осипов Олег Васильевич
// Copyright © 2019 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем. Все права защищены.

#include "JSphere.h"
#include <math.h>


JSphere::JSphere()
{

}


JSphere::JSphere(float x0, float y0, float z0, float radius)
{
    _center = QVector3D(x0, y0, z0);
    _radius = radius;
    is_selecting = false;
}


bool JSphere::is_inside(const JPlane* planes, int nPlanes) const
{
    for (int i=0; i < nPlanes; i++)
    {
        // Если расстояние от центра сферы до какой-либо плоскости меньше радиуса, то сфера пересекает плоскость,
        // и значит не находится внутри усечённой пирамиды выделения
        if (planes[i].distanceTo(_center) < _radius)
            return false;
    }
    return true;
}


float JSphere::radius() const
{
    return _radius;
}

float JSphere::x0() const
{
    return _center.x();
}

float JSphere::y0() const
{
    return _center.y();
}

float JSphere::z0() const
{
    return _center.z();
}

QVector3D JSphere::center() const
{
    return _center;
}

int JSphere::intersects(const JRay &ray, QVector3D *R) const
{
    QVector3D A = ray.A(), B = ray.B();
    QVector3D D = B - A, C = A - _center;
    // d2 - квадрат длины вектора D, c2 - квадрат длины вектора C, cd - скалярное произведение векторов C и D
    double d2 = D.lengthSquared(), c2 = C.lengthSquared(), cd = QVector3D::dotProduct(C, D);
    // Вычисляем коэффициенты квадратного уравнения
    double a = d2, b = 2 * cd, c = c2 - _radius * _radius;
    double rd = b * b - 4 * a * c; // Дискриминант
    if (rd == 0) // Луч касается сферы в одной точке
    {
        double t = -b / (2 * a);
        R[0] = A + D * t;
        return 1;
    }
    if (rd > 0)  // Луч пересекает сферу в двух точках
    {
        rd = sqrt(rd);
        double t1 = (-b + rd) / (2 * a);
        double t2 = (-b - rd) / (2 * a);
        R[0] = A + D * t1;
        R[1] = A + D * t2;
        return 2;
    }
    else
    {
        // Корней нет. Сфера и луч не пересекаются
        return 0;
    }
}

bool JSphere::isCrossed(const JRay &ray)
{
    return _center.distanceToLine(ray.A(), ray.direction()) < _radius;
}