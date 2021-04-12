#ifndef MainGLWidget_H
#define MainGLWidget_H


#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMatrix4x4>
#include "geometry/JPlane.h"
#include "geometry/JSphere.h"
#include "geometry/JCube.h"

class MainGLWidget : public QOpenGLWidget {

public:
    MainGLWidget(QWidget *);

private:

    // Глубина объекта
    float zoffset = 3;

    // Матрица поворота, используемая для поворота сцены, когда пользователь зажимает кнопку мыши
    QMatrix4x4 rotateMatrix;

    // Произведение видовой матрицы и матрицы проектирования
    QMatrix4x4 Q;

    // Матрица, обратная матрице Q
    QMatrix4x4 IQ;

    // Позиция указателя мыши
    QPoint mousePosition;

    // Параметры селектирующего луча
    //JRay selectionRay;

    // Примитивы
    JCube cubes[7];

    JSphere spheres[4];

    // Текст, который выводится на экране
    QString output_text;

    // Инициализация освещения
    void initLighting();

    // Подпрограмма для рисования куба
    void drawCube(const JCube& cube);

    // Процедура для изменения матрицы проектирования
    void resetProjectionMatrix();

    // Процедура для изменения видовой матрицы
    void resetModelViewMatrix();

    ///
    /// \brief toOpenGLScreen
    /// \param pos - позиция указателя мыши в оконной системе координат
    /// \return Возвращает координаты указателя мыши в экранной системе координат OpenGL (от -1 до 1), соответствующую точке pos
    ///
    QPointF toOpenGLScreen(QPoint pos) const;

    ///
    /// \brief depth
    /// \param A
    /// \return Возвращает глубину точки (расстояние до экранной плоскости)
    /// Эту функцию можно использовать для определения того, какой из выделенных объектов ближе к наблюдателю,
    /// и, например, сортировать объекты по глубине
    float depth(QVector3D A);

    ///
    /// \brief selectionRay
    /// \return Возвращает параметры селектирующего луча, выпущенного вглубь экрана из точки P(X, Y)
    /// Для расчёта параметров луча используются текущие матрицы OpenGL
    JRay selectionRay(const QPoint& P) const;

    // Процедура для изменения матрицы поворота
    static void changeRotateMatrix(QMatrix4x4& rotateMatrix, float dx, float dy);

    // Процедура для инициализации параметров OpenGL
    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mouseMoveEvent(QMouseEvent* m_event);
    void wheelEvent(QWheelEvent* w_event);
    void keyPressEvent(QKeyEvent* key_event);

    // Обработчик нажатия кнопок мыши
    void mousePressEvent(QMouseEvent* m_event);
};

#endif // MainGLWidget_H