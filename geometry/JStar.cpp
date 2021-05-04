//
// Created by sdont on 04.05.2021.
//

#include <QMatrix4x4>
#include "JStar.h"

JStar::JStar() {
    modelViewMatrix.setToIdentity();
}

JStar::~JStar() {
    delete[] points;
}

void JStar::init() {
    const float PI = acos(-1);
    points = new QVector3D[n * 5];

    float x1, y1, x2, y2, x3, y3;
    float step = 2 * PI / n;
    for (int i = 0; i < n; i++) {
        x1 = radiusOuter * cosf(i * step);
        y1 = radiusOuter * sinf(i * step);

        x2 = radiusInner * cosf(i * step + step / 2);
        y2 = radiusInner * sinf(i * step + step / 2);

        points[i * n] = QVector3D(x1, y1, 0.0f);
        points[i * n + 1] = QVector3D(x2, y2, 0.0f);
        points[i * n + 2] = QVector3D(0.0f, 0.0f, height);
        polygons.push_back(JPolygon(&points[i*n + 2],&points[i*n], &points[i*n + 1]));

        points[i * n + 3] = QVector3D(0.0f, 0.0f, -height);
        polygons.push_back(JPolygon(&points[i*n], &points[i*n + 3], &points[i*n + 1]));

        x3 = radiusInner * cosf(i * step - step / 2);
        y3 = radiusInner * sinf(i * step - step / 2);

        points[i * n + 4] = QVector3D(x3, y3, 0.0f);
        polygons.push_back(JPolygon(&points[i*n + 4], &points[i*n], &points[i*n + 2]));
        polygons.push_back(JPolygon(&points[i*n + 4], &points[i*n + 3], &points[i*n]));
    }

    for (int i = 0; i < polygons.size(); i++) {
        QVector3D a[3];
        polygons[i].getVertices((float *) a);
        vertices.push_back(a[0]);
        vertices.push_back(a[1]);
        vertices.push_back(a[2]);

        QVector3D b[3];
        polygons[i].getNormals((float *) b);
        normals.push_back(b[0]);
        normals.push_back(b[1]);
        normals.push_back(b[2]);
    }

}

int JStar::intersects(const JRay &ray, QVector3D *R) const {
    QVector3D C;
    int k = 0;
    for (int i = 0; i < polygons.size(); i++) {
        // Если луч пересекает хотя бы одну грань, то считаем, что луч пересёк куб
        // В R записываем координаты точек пересечения с гранями
        if (polygons[i].intersects(ray, C))
            R[k++] = C;
    }
    return k;
}

//Инициализация переменных для вычисление глубины
void JStar::updateDepth(QMatrix4x4 projectMatrix) {
    // Умножим матрицы проектирования и видовую матрицы
    Q = projectMatrix * modelViewMatrix;

    // Вычислим обратную матрицу
    IQ = Q.inverted();
}