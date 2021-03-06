//
// Created by sdont on 05.03.2021.
//

#ifndef OPENGL_OPENGLWIDGET_H
#define OPENGL_OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QPainter>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QtOpenGLWidgets/QOpenGLWidget>

class OpenGLWidget : public QOpenGLWidget {
public:
    explicit OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
        shaderProgram = new QOpenGLShaderProgram();
    }
    ~OpenGLWidget() {
        delete shaderProgram;
    }

private:
    void initializeGL() override;
    void resizeGL(int nWidth, int nHeight) override;
    void paintGL() override;
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

private:
    // Глубина объекта
    float zoffset = 3;

    // Матрица поворота
    QMatrix4x4 rotateMatrix; // Изначально матрица поворота равна единичной матрице

    // Позиция указателя мыши
    QPoint mousePosition;

    // Матрица видового преобразования
    QMatrix4x4 modelViewMatrix;

    // Матрица проектирования
    QMatrix4x4 projectMatrix;

    // Сборщик шейдерных подпрограмм
    QOpenGLShaderProgram *shaderProgram;
};


#endif //OPENGL_OPENGLWIDGET_H
