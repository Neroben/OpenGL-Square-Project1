// Автор: Осипов Олег Васильевич
// Автор: Донцов Александр Алексеевич
// Copyright © 2020 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем.
// Дата изменения: 14.03.2020



#include "OpenGLWidget.h"

void OpenGLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);

    // Фоновый цвет
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Режим рисования только лицевых граней
    glEnable(GL_CULL_FACE);

    // Сборка шейдеров
    if(!shaderProgram.link())
        qDebug() << shaderProgram.log();

    resetModelView();
    initShader();
}

void OpenGLWidget::initShader()
{
    // Текст вершинного шейдера
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":resources/vertexShader.vsh");

    // Текст фрагментного шейдера
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":resources/fragmentShader.fsh");

    if (!shaderProgram.link())
        qDebug() << shaderProgram.log();

    // Создание идентификатора массива вершин
    vertexLocation = shaderProgram.attributeLocation("vertex");

    // Создание идентификатора массива вершин
    matrixLocation = shaderProgram.uniformLocation("matrix");

    // Идентификатор массива цветов
    colorLocation =  shaderProgram.uniformLocation("color");
}

void OpenGLWidget::resizeGL(int nWidth, int nHeight)
{
    // Задание области вывода
    glViewport(0, 0, nWidth, nHeight);
    // Задаём матрицу центрального проектирования
    resetProjection();
}

// Внутри данной подпрограммы происходит рисование объектов
void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //рисование куба
    glCube();

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

// Подпрограмма для рисования куба в начале координат
void OpenGLWidget::glCube()
{
    // Массив из 8 вершин куба
    static const float vertices[8][3] =
            {{ 1.0f,  1.0f,  1.0f}, // Координаты первой вершины
             { 1.0f,  1.0f, -1.0f}, // Координаты второй вершины
             { 1.0f, -1.0f,  1.0f},
             { 1.0f, -1.0f, -1.0f},
             {-1.0f,  1.0f,  1.0f},
             {-1.0f,  1.0f, -1.0f},
             {-1.0f, -1.0f,  1.0f},
             {-1.0f, -1.0f, -1.0f}}; // Координаты восьмой вершины

    // Массив индексов вершин vertices
    static const GLushort indeces[] =
            { 4, 5, 7, 6,   // Индексы вершин первой грани
              2, 3, 1, 0,   // Индексы вершин второй грани
              0, 1, 5, 4,   // ...
              6, 7, 3, 2,
              4, 6, 2, 0,
              1, 3, 7, 5 }; // Индексы вершин шестой грани

    shaderProgram.bind();

    shaderProgram.setUniformValue(matrixLocation, projectMatrix*modelViewMatrix);

    shaderProgram.setUniformValue(colorLocation, QColor(255, 200, 14));

    shaderProgram.setAttributeArray(vertexLocation, (float*)vertices, 3);

    shaderProgram.enableAttributeArray(vertexLocation);

    glDrawElements(GL_QUADS, 6*4, GL_UNSIGNED_SHORT, indeces);

    shaderProgram.disableAttributeArray(vertexLocation);

    shaderProgram.release();
}


void OpenGLWidget::resetProjection()
{
    // Инициализация единичной матрицы
    projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    projectMatrix.perspective(30.0, (float)width() / (float)height(), 0.1, 20);
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
    changeRotateMatrix(rotateMatrix, (float) dp.x(), (float) dp.y());
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
    zoffset -= (float)(w_event->angleDelta().x() + w_event->angleDelta().y()) / 500.0f;
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
