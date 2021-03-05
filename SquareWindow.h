//
// Created by sdont on 05.03.2021.
//

#ifndef OPENGL_SQUAREWINDOW_H
#define OPENGL_SQUAREWINDOW_H

#include <QGuiApplication>
#include <QMatrix4x4>
#include <QScreen>
#include <QOpenGLShaderProgram>
#include "OpenGLWindow.h"

class SquareWindow : public OpenGLWindow {
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    QOpenGLShaderProgram *m_program = nullptr;
    int m_frame = 0;
};


#endif //OPENGL_SQUAREWINDOW_H
