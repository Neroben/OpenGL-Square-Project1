// Автор: Осипов Олег Васильевич
// Copyright © 2020 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем.
// Дата изменения: 14.03.2020

// Приложение, использующее простейший шейдер для рисования куба с разноцветными гранями

#include "MainGLWidget.h"

//MainGLWidget::MainGLWidget(QWidget *parent)
//        : QGLWidget(QGLFormat(), parent)
//{
//    setWindowTitle("Трёхмерная графика. Куб с разноцветными гранями");
//    //setWindowState(Qt::WindowFullScreen); // Разворачиваем приложение на весь экран
//}

void MainGLWidget::initializeGL()
{
    // Включение сортировки по глубине
    glEnable(GL_DEPTH_TEST);

    // Рисуем только лицевые грани
    glEnable(GL_CULL_FACE);

    // Инициализируем видовую матрицу
    resetModelView();

    initShader();
}



void MainGLWidget::initShader(void)
{
    // Текст вершинного шейдера
    shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
                                           "#version 330 \n"
                                           "attribute vec4 vertex;\n"
                                           "uniform mat4 matrix;\n"
                                           "in vec4 color;\n"
                                           "out vec4 varyingColor;\n"
                                           "void main(void)\n"
                                           "{\n"
                                           "   varyingColor = color;\n"
                                           "   gl_Position = matrix * vertex;\n"
                                           "}");

    // Текст фрагментного шейдера
    shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
                                           "#version 330 \n"
                                           "in vec4 varyingColor;\n"
                                           "out vec4 fragColor;\n"
                                           "void main(void)\n"
                                           "{\n"
                                           "   fragColor = varyingColor;\n"
                                           "}");

    if (shaderProgram.link() == false)
        qDebug() << shaderProgram.log();

    // Создание идентификатора массива вершин
    vertexLocation = shaderProgram.attributeLocation("vertex");

    // Создание идентификатора итоговой матрицы
    matrixLocation = shaderProgram.uniformLocation("matrix");
}


void MainGLWidget::resizeGL(int nWidth, int nHeight)
{
    // Задание области вывода
    glViewport(0, 0, nWidth, nHeight);
    // Задаём матрицу центрального проектирования
    resetProjection();
}



// Внутри данной подпрограммы происходит рисование объектов
void MainGLWidget::paintGL()
{
    // Очистка буфера глубины и буфера цвета
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Рисование куба
    glCube();

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



// Подпрограмма для рисования куба
void MainGLWidget::glCube()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Массив из 24 вершин куба (6 граней, каждая из 4 вершин)
    static const float vertices[6*4][3] =
            {{ -1.0f,  1.0f,  1.0f}, { -1.0f,  -1.0f,  1.0f}, {  1.0f, -1.0f,  1.0f}, {  1.0f,  1.0f,  1.0f},  // Передняя грань (z =  1)
             {  1.0f,  1.0f, -1.0f}, {  1.0f,  -1.0f, -1.0f}, { -1.0f, -1.0f, -1.0f}, { -1.0f,  1.0f, -1.0f},  // Задняя грань   (z = -1)
             {  1.0f,  1.0f, -1.0f}, { -1.0f,   1.0f, -1.0f}, { -1.0f,  1.0f,  1.0f}, {  1.0f,  1.0f,  1.0f},  // Верхняя грань  (y =  1)
             {  1.0f, -1.0f,  1.0f}, { -1.0f,  -1.0f,  1.0f}, { -1.0f, -1.0f, -1.0f}, {  1.0f, -1.0f, -1.0f},  // Нижняя грань   (y = -1)
             {  1.0f, -1.0f,  1.0f}, {  1.0f,  -1.0f, -1.0f}, {  1.0f,  1.0f, -1.0f}, {  1.0f,  1.0f,  1.0f},  // Правая грань   (x =  1)
             { -1.0f,  1.0f,  1.0f}, { -1.0f,   1.0f, -1.0f}, { -1.0f, -1.0f, -1.0f}, { -1.0f, -1.0f,  1.0f}}; // Левая грань    (x = -1)

    // Массив цветов для каждой вершины
    static const float colors[6*4][3] =
            { { 1.0f, 0.5f, 0.2f}, { 1.0f, 0.5f, 0.2f}, { 1.0f, 0.5f, 0.2f}, { 1.0f, 0.5f, 0.7f},   // Цвет передней грани
              { 1.0f, 0.5f, 0.2f}, { 1.0f, 0.5f, 0.2f}, { 1.0f, 0.5f, 0.2f}, { 1.0f, 0.5f, 0.7f},   // Цвет задней грани
              { 0.8f, 0.9f, 0.1f}, { 0.8f, 0.9f, 0.1f}, { 0.8f, 0.9f, 0.1f}, { 0.8f, 0.9f, 0.1f},   // Цвет верхней грани
              { 0.8f, 0.9f, 0.1f}, { 0.8f, 0.9f, 0.1f}, { 0.8f, 0.9f, 0.1f}, { 0.8f, 0.9f, 0.1f},   // Цвет нижней грани
              { 0.0f, 0.6f, 0.9f}, { 0.0f, 0.6f, 0.9f}, { 0.0f, 0.6f, 0.9f}, { 0.0f, 0.2f, 1.0f},   // Цвет правой грани
              { 0.0f, 0.6f, 0.9f}, { 0.0f, 0.6f, 0.9f}, { 0.0f, 0.6f, 0.9f}, { 0.0f, 0.2f, 1.0f} }; // Цвет левой грани

    shaderProgram.bind();

    // Зададим матрицу, на которую будут умножены однородные координаты вершин в вершинном шейдере
    shaderProgram.setUniformValue(matrixLocation, projectMatrix*modelViewMatrix);

    // Передаём массив вершин (координаты каждой вершины задаются тремя числами)
    shaderProgram.setAttributeArray(vertexLocation, (float*)vertices, 3);

    // Передаём массив цветов каждой вершины (цвет каждой вершины задаётся тремя числами)
    shaderProgram.setAttributeArray("color", (float*)colors, 3);

    shaderProgram.enableAttributeArray("color");

    shaderProgram.enableAttributeArray(vertexLocation);

    // Рисование 6 граней куба, координаты и цвета которых заданы в массиве. Всего массив содержит 24 вершины
    glDrawArrays(GL_QUADS, 0, 6*4);

    shaderProgram.disableAttributeArray(vertexLocation);

    shaderProgram.disableAttributeArray("color");

    shaderProgram.release();
}



void MainGLWidget::resetProjection()
{
    // Инициализация матрицы проектирования как единичной матрицы
    projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    projectMatrix.perspective(30.0, (float)width() / height(), 0.1, 20);
}



// Процедура для изменения видовой матрицы
void MainGLWidget::resetModelView()
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
void MainGLWidget::mouseMoveEvent(QMouseEvent* m_event)
{
    // Вычислим, на сколько переместился курсор мыши между двумя событиями mouseMoveEvent
    QPoint dp = m_event->pos() - mousePosition;
    // Изменим матрицу поворота в соответствии с тем, как пользователь переместил курсор мыши
    changeRotateMatrix(rotateMatrix, dp.x(), dp.y());
    // Сохраним текущую позицию мыши
    mousePosition = m_event->pos();
    // Обновим матрицу аффинных преобразований
    resetModelView();
    update(); // Перерисовать окно
}


// Процедура предназначена для изменения матрицы поворота, чтобы куб поворачивался в нужном направлении строго вслед за указателем мыши.
// Вызывается, когда пользователь изменил положение указателя мыши при зажатой кнопке (мыши)
void MainGLWidget::changeRotateMatrix(QMatrix4x4& R, float dx, float dy)
{
    R.rotate(-dx, 0, 1, 0);         // Умножение R на матрицу поворота вокруг оси y
    R.rotate(-dy, 1, 0, 0);         // Умножение R на матрицу поворота вокруг оси x
}


// Обработчик события прокрутки колеса мыши
void MainGLWidget::wheelEvent(QWheelEvent* w_event)
{
    // При прокрутке колеса мыши изменяем глубину объекта
    zoffset -= (float)(w_event->angleDelta().x() + w_event->angleDelta().y()) / 500.0f;
    resetModelView(); // Обновим матрицу аффинных преобразований
    update(); // Перерисовать окно
}


void MainGLWidget::mousePressEvent(QMouseEvent* m_event)
{
    mousePosition = m_event->pos();
}


void MainGLWidget::keyPressEvent(QKeyEvent* event)
{
    // Закрыть окно при нажатии клавиши Escape
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
}
