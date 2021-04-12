//
// Created by sdont on 05.04.2021.
//

#ifndef OPENGL_JRAY_H
#define OPENGL_JRAY_H


#include <QVector3D>

class JRay {
    QVector3D _A, _B;

    ///
    /// \brief Единичный вектор направления
    ///
    QVector3D _D;
public:

    JRay();

    ///
    /// \brief JRay - конструктор луча по двум точкам в пространстве
    /// \param A - координаты первой точки в пространстве
    /// \param B - координаты второй точки в пространстве
    ///
    JRay(QVector3D A, QVector3D B);

    ///
    /// \brief A
    /// \return Возвращает координаты начальной точки луча
    ///
    QVector3D A() const;

    ///
    /// \brief B
    /// \return Возвращает координаты второй точки луча
    ///
    QVector3D B() const;

    ///
    /// \brief direction
    /// \return Возвращает единичный вектор направления
    ///
    QVector3D direction() const;

    bool isNull();
};


#endif //OPENGL_JRAY_H
