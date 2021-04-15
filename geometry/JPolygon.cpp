//
// Created by sdont on 13.04.2021.
//

#include "JPolygon.h"

JPolygon::JPolygon()
{

}

void JPolygon::initNormal()
{
    QVector3D* V1 = at(0), *V2 = at(1), *V3 = at(2);

    // Вычислим векторное произведение векторов (V2 - V1) и (V3 - V1)
    QVector3D N = QVector3D::crossProduct(*V2 - *V1, *V3 - *V1);

    // Вычислим уравнение прямой по вектору нормали N и одной точке V1
    plane = JPlane(N.x(), N.y(), N.z(), -QVector3D::dotProduct(N, *V1));
}

JPolygon::JPolygon(QVector3D *V1, QVector3D *V2, QVector3D *V3)
{
    push_back(V1);
    push_back(V2);
    push_back(V3);
    initNormal();
    //sort_vertices();
}

JPolygon::JPolygon(QVector3D* V1, QVector3D* V2, QVector3D* V3, QVector3D* V4)
{
    push_back(V1);
    push_back(V2);
    push_back(V3);
    push_back(V4);
    initNormal();
    //sort_vertices();
}

// Сортировка вершин по часовой стрелке
void JPolygon::sort_vertices()
{
    QVector3D** V = data();
    struct CrossProduct
    {
        QVector3D O;
        static double X(const QVector3D& A, const QVector3D& B) { return A.y()*B.z() - A.z()*B.y(); }
        static double Y(const QVector3D& A, const QVector3D& B) { return A.z()*B.x() - A.x()*B.z(); }
        static double Z(const QVector3D& A, const QVector3D& B) { return A.x()*B.y() - A.y()*B.x(); }

        double (*comp) (const QVector3D& A, const QVector3D& B);

        CrossProduct(const JPlane& plane, const QVector3D& O)
        {
            QVector3D normal = plane.normal();
            // Ищем максимальную по модулю координату вектора нормали к плоскости _plane
            comp = CrossProduct::X;
            double maxk  = fabs(normal.x());
            double normb = fabs(normal.y());
            if (normb > maxk) {
                comp = CrossProduct::Y;
                maxk = normb;
            }
            if(fabs(normal.z()) > maxk)
                comp = CrossProduct::Z;
            this->O = O;
        }

        bool operator()(const QVector3D* V1, const QVector3D* V2) const
        {
            QVector3D A = *V1 - O, B = *V2 - O;
            return comp(A, B) < 0;
        }

    } prod(plane, *(V[0]));

    std::sort(V + 1, V + size(), prod);
}

// Для уменьшения количества арифметических операций функция проецирует многоугольник на одну из плоскостей: X-Y, X-Z или Y-Z и затем вычисляет только одну (из трёх) координату векторного произведения векторов
// Выбор плоскости определяется исходя из того, какой из трёх углов между осями координат и вектором нормали N к плоскости минимальный. Для этого достаточно найти максимум из модулей N.x, N.y, N.z
// К примеру, если минимальный угол - это угол между вектором нормали N и осью x, то многоугольник проецируется на плоскость Y-Z. При этом |N.x| > |N.y| > |N.z|
// Вершины обязательно должны быть отсортированы по/против часовой стрелки относительно центра многоугольника!!!
bool JPolygon::inside(const QVector3D &E) const
{
    int i, L = size();
    QVector3D N = plane.normal();
    float fA = fabs(N.x()), fB = fabs(N.y()), fC = fabs(N.z());
    double x1, x2, y1, y2, z1, z2;
    // Для проверки нахождения точки R внутри многоугольника вычисляются векторные произведения векторов
    if(fA >= fB && fA >= fC) // Проекция на плоскость Y-Z
    {
        y1 = at(L-1)->y() - E.y(); z1 = at(L-1)->z() - E.z();
        y2 = at(0)->y() - E.y(); z2 = at(0)->z() - E.z();
        bool sign = y1 * z2 - z1 * y2 > 0;
        y1 = y2; z1 = z2;
        for (i=1; i < L; i++)
        {
            y2 = at(i)->y() - E.y();
            z2 = at(i)->z() - E.z();
            if (sign != y1*z2 - z1*y2 > 0)
                return false;
            y1 = y2;
            z1 = z2;
        }
        return true;
    }
    if(fB >= fA && fB >= fC) // Проекция на плоскость X-Z
    {
        x1 = at(L-1)->x() - E.x(); z1 = at(L-1)->z() - E.z();
        x2 = at(0)->x() - E.x(); z2 = at(0)->z() - E.z();
        bool sign = x1 * z2 - z1 * x2 > 0;
        x1 = x2; z1 = z2;
        for (i=1; i < L; i++)
        {
            x2 = at(i)->x() - E.x();
            z2 = at(i)->z() - E.z();
            if (sign != x1*z2 - z1*x2 > 0)
                return false;
            x1 = x2;
            z1 = z2;
        }
        return true;
    }
    if(fC >= fA && fC >= fB) // Проекция на плоскость X-Y
    {
        x1 = at(L-1)->x() - E.x(); y1 = at(L-1)->y() - E.y();
        x2 = at(0)->x() - E.x(); y2 = at(0)->y() - E.y();
        bool sign = x1 * y2 - y1 * x2 > 0;
        x1 = x2; y1 = y2;
        for (i=1; i < L; i++)
        {
            x2 = at(i)->x() - E.x();
            y2 = at(i)->y() - E.y();
            if (sign != x1*y2 - y1*x2 > 0)
                return false;
            x1 = x2;
            y1 = y2;
        }
        return true;
    }
}


bool JPolygon::intersects(const JRay &ray, QVector3D& R) const
{
    // Если луч ортогонален плоскости многоугольника, то искать точку персечения не нужно
    if (QVector3D::dotProduct(ray.direction(), normal()) == 0)
    {
        return false;
    }
    else
    {
        R = plane.intersects(ray);
        return inside(R);
    }
}



int JPolygon::nVertices() const
{
    return size();
}



QVector3D JPolygon::operator [](int index) const
{
    return *at(index);
}


QVector3D JPolygon::normal() const
{
    return plane.normal();
}

void JPolygon::getVertices(float* vertices) {
    for(int i = 0; i < this->nVertices(); i++) {
        vertices[i*3] = (*this)[i].x();
        vertices[i*3 + 1] = (*this)[i].y();
        vertices[i*3 + 2] = (*this)[i].z();
    }
}