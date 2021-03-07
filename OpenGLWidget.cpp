//
// Created by sdont on 05.03.2021.
//


#include "OpenGLWidget.h"

void OpenGLWidget::initializeGL() {
    f = QOpenGLContext::currentContext()->functions();
    f->glEnable(GL_DEPTH_TEST);

    // Фоновый цвет
    f->glClearColor(0.0f, 0.5f, 0.0f, 0.0f);

    // Инициализация шейдеров
    // Вершинный шейдер
    // Шейдер принимает на вход вектор из двух координат (x, y)
    // При расчёте выходного вектора зададим координаты z = 0, w = 1
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                          "attribute vec2 vertex;\n"
                                          "uniform mat4 matrix;\n"
                                          "void main(void)\n"
                                          "{\n"
                                          "   gl_Position = matrix * vec4(vertex.xy, 0.0, 1.0);\n"
                                          "}");

    // Фрагментный шейдер задаёт только цвет
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "uniform mediump vec4 color;\n"
                                           "void main(void)\n"
                                           "{\n"
                                           "   gl_FragColor = color;\n"
                                           "}");

    // Сборка шейдеров
    if(!shaderProgram->link())
        qDebug() << shaderProgram->log();

    // Переместим квадрат вглубь экрана, чтобы он был виден
    zoffset -= 0.5;
    resetModelView();
}

void OpenGLWidget::resizeGL(int nWidth, int nHeight)
{
    // Задание области вывода
    f->glViewport(0, 0, nWidth, nHeight);
    // Задаём матрицу центрального проектирования
    resetProjection();
}

// Внутри данной подпрограммы происходит рисование объектов
void OpenGLWidget::paintGL()
{
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Массив координат (x, y) четырёх вершин квадрата.
    // Координату z зададим в коде вершинного шейдера (z = 0)
    static const float vertices[4*2] = { -1.0f,  1.0f,   // Первая вершина квадрата
                                         -1.0f, -1.0f,   // Вторая вершина квадрата
                                         1.0f, -1.0f,   // Третья вершина квадрата
                                         1.0f,  1.0f }; // Четвёртая вершина квадрата

    shaderProgram->bind();

    // Зададим матрицу, на которую будут умножены однородные координаты вершин в вершинном шейдере
    shaderProgram->setUniformValue("matrix", projectMatrix * modelViewMatrix);

    // Зададим цвет квадрата
    shaderProgram->setUniformValue("color", QColor(255, 128, 40));

    // Передадим шейдеру весь массив вершин
    // Третий параметр равен двум, потому что одна вершина квадрата задана в массиве vertices двумя числами
    shaderProgram->setAttributeArray("vertex", vertices, 2);
    shaderProgram->enableAttributeArray("vertex");

    // Рисование примитива по координатам, заданным в массиве
    // Третий параметр означает, что массив vertices содержит координаты 4 вершин
    f->glDrawArrays(GL_QUADS, 0, 4);

    shaderProgram->disableAttributeArray("vertex");

    shaderProgram->release();

    textOut();
}


void OpenGLWidget::textOut()
{
    // Вывод на экран текста
    QPainter painter(this);
    painter.setPen(Qt::yellow);
    painter.setFont(QFont("Cambria", 10, QFont::Bold));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawText(10, 20, windowTitle());
    painter.drawText(10, 35, "Колесо мыши - удалить/приблизить");
    painter.drawText(10, 50, "Зажатая кнопка мыши - повернуть сцену");
    painter.drawText(10, 65, "Esc - выход");
    painter.end();
}


void OpenGLWidget::resetProjection()
{
    // Инициализация единичной матрицы
    projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    projectMatrix.perspective(30.0, (float)width() / height(), 0.1, 20);
}



// Процедура для изменения видовой матрицы
void OpenGLWidget::resetModelView()
{
    // Инициализация видовой матрицы как единичной
    modelViewMatrix.setToIdentity();

    // Далее аффинные преобразования записаны в обратном порядке

    // Третья операция - перенос объекта вдоль оси z (например, вглубь экрана)
    // Умножим видовую матрицу на матрицу переноса
    modelViewMatrix.translate(0, 0, -zoffset);

    // Вторая операция - поворот объекта
    // Умножим видовую матрицу на матрицу поворота
    modelViewMatrix *= rotateMatrix.transposed();

    // Первая операция - масштабирование объекта (уменьшим объект, чтобы он не занимал весь экран)
    modelViewMatrix.scale(0.3, 0.3, 0.3);
}


// Обработчик события перемещения указателя мыши (событие происходит при зажатой кнопке мыши)
void OpenGLWidget::mouseMoveEvent(QMouseEvent* m_event)
{
//    // Вычислим, на сколько переместился курсор мыши между двумя событиями mouseMoveEvent
    QPoint dp = m_event->pos() - mousePosition;
//    // Изменим матрицу поворота в соответствии с тем, как пользователь переместил курсор мыши
    changeRotateMatrix(rotateMatrix, dp.x(), dp.y());
//    // Сохраним текущую позицию мыши
    mousePosition = m_event->pos();
//    // Обновим матрицу аффинных преобразований
    resetModelView();
    update(); // Перерисовать окно
}


// Процедура предназначена для изменения матрицы поворота, чтобы квадрат поворачивался в нужном направлении строго вслед за указателем мыши.
// Вызывается, когда пользователь изменил положение указателя мыши при зажатой кнопке (мыши)
void OpenGLWidget::changeRotateMatrix(QMatrix4x4& R, float dx, float dy)
{
    R.rotate(-dx, 0, 1, 0);         // Умножение R на матрицу поворота вокруг оси y
    R.rotate(-dy, 1, 0, 0);         // Умножение R на матрицу поворота вокруг оси x
}


// Обработчик события прокрутки колеса мыши
void OpenGLWidget::wheelEvent(QWheelEvent* w_event)
{
    // При прокрутке колеса мыши изменяем глубину объекта
    zoffset -= (w_event->angleDelta().x() + w_event->angleDelta().y()) / 500.0;
    resetModelView(); // Обновим матрицу аффинных преобразований
    update(); // Перерисовать окно
}


void OpenGLWidget::mousePressEvent(QMouseEvent* m_event)
{
    mousePosition = m_event->pos();
}


void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
//     Закрыть окно при нажатии клавиши Escape
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
}
