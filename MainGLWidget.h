//
// Created by sdont on 05.03.2021.
//

#ifndef OPENGL_MAINGLWIDGET_H
#define OPENGL_MAINGLWIDGET_H

#include <QOpenGLFunctions>
#include <QPainter>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include "geometry/JCube.h"

class MainGLWidget : public QOpenGLWidget {
public:
    MainGLWidget(QWidget *parent);
    ~MainGLWidget();
private:
    // Глубина объекта
    float zoffset = 3;

    // Примитивы
    JCube *cubes;

    // Матрица поворота
    QMatrix4x4 rotateMatrix; // Изначально матрица поворота равна единичной матрице

    // Позиция указателя мыши
    QPoint mousePosition;

    // Подпрограмма для рисования куба
    void glCube(JCube* cube);

    // Процедура для изменения матрицы проектирования
    void resetProjection();

    // Процедура для изменения видовой матрицы
    void resetModelView();

    // Процедура для изменения матрицы поворота
    static void changeRotateMatrix(QMatrix4x4& rotate_matrix, float dx, float dy);

    // Инициализация шейдеров
    void initShader(void);

    void initializeGL();
    void resizeGL(int nWidth, int nHeight);
    void paintGL();
    void mouseMoveEvent(QMouseEvent* m_event);
    void wheelEvent(QWheelEvent* w_event);
    void keyPressEvent(QKeyEvent* key_event);
    void mousePressEvent(QMouseEvent* m_event);

    // Матрица видового преобразования
    QMatrix4x4 modelViewMatrix;

    // Матрица проектирования
    QMatrix4x4 projectMatrix;

    // Произведение видовой матрицы и матрицы проектирования
    QMatrix4x4 Q;

    // Матрица, обратная матрице Q
    QMatrix4x4 IQ;

    // Сборщик шейдерных подпрограмм
    QOpenGLShaderProgram shaderProgram;

    int vertexLocation;
    int normalLocation;
    int matrixLocation;
    int normalMatrixLocation;
    int modelViewMatrixLocation;

    void setLighting();

    JRay selectionRay(const QPoint &P) const;

    QPointF toOpenGLScreen(QPoint pos) const;

    float customDepth(QVector3D A);

    void initCubes();
};

#endif //OPENGL_MAINGLWIDGET_H
