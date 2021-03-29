#ifndef MAINGLWIDGET_H
#define MAINGLWIDGET_H

#include <QOpenGLFunctions>
#include <QPainter>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
//#include <QtOpenGLWidgets/QOpenGLWidget>

class MainGLWidget : public QOpenGLWidget
{
public:

    MainGLWidget(QWidget *parent);

private:

    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mouseMoveEvent(QMouseEvent* m_event);
    void wheelEvent(QWheelEvent* w_event);
    void keyPressEvent(QKeyEvent* key_event);
    void mousePressEvent(QMouseEvent* m_event);

    // Процедура для изменения матрицы проектирования
    void resetProjection();

    // Процедура для изменения видовой матрицы
    void resetModelView();

    // Процедура для изменения матрицы поворота
    static void changeRotateMatrix(QMatrix4x4& rotate_matrix, float dx, float dy);

    // Процедура выводит на экран текст (подсказку)
    void textOut();

    void swapColors(void);

private:
    // Глубина объекта
    float zoffset = 3;

    // Цвет вершин
    QVector4D color[4];

    // Матрица поворота
    QMatrix4x4 rotateMatrix; // Изначально матрица поворота равна единичной матрице

    // Позиция указателя мыши
    QPoint mousePosition;

    // Матрица видового преобразования
    QMatrix4x4 modelViewMatrix;

    // Матрица проектирования
    QMatrix4x4 projectMatrix;

    // Сборщик шейдерных подпрограмм
    QOpenGLShaderProgram shaderProgram;
};


#endif // MAINGLWIDGET_H
