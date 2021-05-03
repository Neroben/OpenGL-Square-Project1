// Автор: Осипов Олег Васильевич
// Copyright © 2020 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем.
// Дата изменения: 18.04.2021

// Приложение, использующее простейший шейдер для освещения куба

#include "MainGLWidget.h"

MainGLWidget::~MainGLWidget() {

}

MainGLWidget::MainGLWidget(QWidget *parent) : QOpenGLWidget(parent), shaderProgram() {
    setWindowTitle("Трёхмерная графика. Селектирование куба");
    //setWindowState(Qt::WindowFullScreen); // Разворачиваем приложение на весь экран
    initCubes();
    sphere = JSphere();
    sphere.init();
}

void MainGLWidget::initCubes() {

    QVector3D A(-1.0f, -1.0f, -1.0f);
    QVector3D B(1.0f, 1.0f, 1.0f);

    cubes[0].init(A, B);
    cubes[0].center = QVector3D(-1.0f, 0.0f, 0.0f);
    cubes[1].init(A, B);
    cubes[1].center = QVector3D(1.0f, 0.0f, 0.0f);
}

void MainGLWidget::initializeGL() {
    glClearDepth(1.0);    // Разрешить очистку буфера глубины
    glDepthFunc(GL_LESS);  // Тип теста глубины

    // Режим рисования только лицевых граней
    glEnable(GL_CULL_FACE);

    // Инициализируем видовую матрицу
    resetModelView();

    initShader();
}


void MainGLWidget::initShader(void) {

    // Текст вершинного шейдера
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/vertexLightingShader.vsh");

    // Текст фрагментного шейдера
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/fragmentLightingShader.fsh");

    if (shaderProgram.link() == false)
        qDebug() << shaderProgram.log();

    // Создание идентификатора массива вершин
    vertexLocation = shaderProgram.attributeLocation("vertex");

    // Создание идентификатора массива нормалей к вершинам
    normalLocation = shaderProgram.attributeLocation("normal");

    // Создание идентификатора матрицы
    matrixLocation = shaderProgram.uniformLocation("matrix");

    // Идентификатор матрицы для пересчёта векторов нормалей
    normalMatrixLocation = shaderProgram.uniformLocation("normalMatrix");

    // Идентификатор видовой матрицы, которая будет использована для расчёта освещения
    modelViewMatrixLocation = shaderProgram.uniformLocation("modelViewMatrix");
}


void MainGLWidget::setLighting() {

    // Цвет фонового освещения
    shaderProgram.setUniformValue("ambientColor", QVector3D(0.0, 0.2, 0.0));

    // Цвет самого источника
    shaderProgram.setUniformValue("diffuseColor", QVector3D(0.6, 0.6, 0.5));

    // Позиция источника света
    shaderProgram.setUniformValue("lightPos", QVector3D(0.0, 0.0, -2.5));
}


void MainGLWidget::resizeGL(int nWidth, int nHeight) {

    // Задание области вывода
    glViewport(0, 0, nWidth, nHeight);
    // Задаём матрицу центрального проектирования
    resetProjection();
}


// Внутри данной подпрограммы происходит рисование объектов
void MainGLWidget::paintGL() {
    glEnable(GL_DEPTH_TEST);

    // Очистка буфера глубины и буфера цвета
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();

    // Устанавливаем параметры освещения
    setLighting();

    for (int i = 0; i < 2; i++) {
        // Рисование куба
        drawCube(cubes[i]);
    }

    drawSphere(sphere);

    shaderProgram.release();

    // Вывод на экран текста
    QPainter painter(this);
    painter.setPen(Qt::yellow);
    painter.setFont(QFont("Cambria", 10, QFont::Bold));
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.drawText(10, 20, windowTitle());
    painter.drawText(10, 35, "Колесо мыши - удалить/приблизить");
    painter.drawText(10, 50, "Зажатая кнопка мыши - повернуть сцену");
    painter.drawText(10, 65, depthInfo);
    painter.drawText(10, 80, "Escape - выход");
    painter.end();
}


// Подпрограмма для рисования куба
void MainGLWidget::drawCube(const JCube &cube) {

    // Зададим матрицу, на которую будут умножены однородные координаты вершин в вершинном шейдере
    shaderProgram.setUniformValue(matrixLocation, projectMatrix * cube.modelViewMatrix);

    // Матрица, используемая для пересчёта векторов нормалей в вершинном шейдере
    shaderProgram.setUniformValue(normalMatrixLocation, cube.modelViewMatrix.normalMatrix());

    // Видовая матрица для вершинного шейдера
    shaderProgram.setUniformValue(modelViewMatrixLocation, cube.modelViewMatrix);

    // Цвет куба
    QVector3D cubeColor;
    if (cube.is_selecting)
        cubeColor = QVector3D(1.0, 0.5, 0.3);
    else
        cubeColor = QVector3D(0.0, 0.5, 0.9);

    shaderProgram.setUniformValue("objectColor", cubeColor);

    // Передаём массив вершин (координаты каждой вершины задаются тремя числами)
    shaderProgram.setAttributeArray(vertexLocation, cube.vertices);

    // Передаём массив векторов нормалей к вершинам vertices. Каждый вектор состоит из трёх чисел
    shaderProgram.setAttributeArray(normalLocation, cube.normales);

    shaderProgram.enableAttributeArray(vertexLocation);

    shaderProgram.enableAttributeArray(normalLocation);

    // Рисование 6 граней куба, координаты и нормали которых заранее переданы в массивах. Всего массивы содержит 24 вершины
    glDrawArrays(GL_QUADS, 0, 6 * 4);

    shaderProgram.disableAttributeArray(vertexLocation);

    shaderProgram.disableAttributeArray(normalLocation);

}

// Подпрограмма для рисования куба
void MainGLWidget::drawSphere(const JSphere &sphere) {

    // Зададим матрицу, на которую будут умножены однородные координаты вершин в вершинном шейдере
    shaderProgram.setUniformValue(matrixLocation, projectMatrix * sphere.modelViewMatrix);

    // Матрица, используемая для пересчёта векторов нормалей в вершинном шейдере
    shaderProgram.setUniformValue(normalMatrixLocation, sphere.modelViewMatrix.normalMatrix());

    // Видовая матрица для вершинного шейдера
    shaderProgram.setUniformValue(modelViewMatrixLocation, sphere.modelViewMatrix);

    // Цвет куба
    QVector3D cubeColor;
    if (sphere.is_selecting)
        cubeColor = QVector3D(1.0, 0.5, 0.3);
    else
        cubeColor = QVector3D(0.0, 0.5, 0.9);

    shaderProgram.setUniformValue("objectColor", cubeColor);

    // Передаём массив вершин (координаты каждой вершины задаются тремя числами)
    shaderProgram.setAttributeArray(vertexLocation, sphere.vertices.data());

    // Передаём массив векторов нормалей к вершинам vertices. Каждый вектор состоит из трёх чисел
    shaderProgram.setAttributeArray(normalLocation, sphere.normals.data());


    shaderProgram.enableAttributeArray(vertexLocation);
    shaderProgram.enableAttributeArray(normalLocation);

    glDrawArrays(GL_TRIANGLES, 0, sphere.vertices.size());

    shaderProgram.disableAttributeArray(vertexLocation);

    shaderProgram.disableAttributeArray(normalLocation);

}

void MainGLWidget::resetProjection() {
    // Инициализация единичной матрицы
    projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    projectMatrix.perspective(30.0, (float) width() / height(), 0.1, 20);
}

void MainGLWidget::resetModelView() {
    for (int i = 0; i < 2; i++) {
        resetModelViewCube(&cubes[i]);
    }
    resetModelViewSphere(&sphere);
}

// Процедура для изменения видовой матрицы
void MainGLWidget::resetModelViewSphere(JSphere *sphere) {
    // Инициализация видовой матрицы как единичной
    sphere->modelViewMatrix.setToIdentity();

    // Далее аффинные преобразования записаны в обратном порядке

    // Четвертая операция - перенос объекта вдоль оси z (например, вглубь экрана)
    // Умножим видовую матрицу на матрицу переноса
    sphere->modelViewMatrix.translate(0, 0, -zoffset);

    // Третья операция - поворот объекта
    // Умножим видовую матрицу на матрицу поворота
    sphere->modelViewMatrix *= rotateMatrix.transposed();

    // Вторая операция - перенос объекта на его место
    // Умножим видовую матрицу на матрицу переноса
//    sphere->modelViewMatrix.translate(sphere->center());

    // Первая операция - масштабирование объекта (уменьшим объект, чтобы он не занимал весь экран)
    sphere->modelViewMatrix.scale(0.3f, 0.3f, 0.3f);

    sphere->updateDepth(projectMatrix);
}

// Процедура для изменения видовой матрицы
void MainGLWidget::resetModelViewCube(JCube *cube) {
    // Инициализация видовой матрицы как единичной
    cube->modelViewMatrix.setToIdentity();

    // Далее аффинные преобразования записаны в обратном порядке

    // Четвертая операция - перенос объекта вдоль оси z (например, вглубь экрана)
    // Умножим видовую матрицу на матрицу переноса
    cube->modelViewMatrix.translate(0, 0, -zoffset);

    // Третья операция - поворот объекта
    // Умножим видовую матрицу на матрицу поворота
    cube->modelViewMatrix *= rotateMatrix.transposed();

    // Вторая операция - перенос объекта на его место
    // Умножим видовую матрицу на матрицу переноса
    cube->modelViewMatrix.translate(cube->center);

    // Первая операция - масштабирование объекта (уменьшим объект, чтобы он не занимал весь экран)
    cube->modelViewMatrix.scale(0.3f, 0.3f, 0.3f);

    cube->updateDepth(projectMatrix);
}


// Обработчик события перемещения указателя мыши (событие происходит при зажатой кнопке мыши)
void MainGLWidget::mouseMoveEvent(QMouseEvent *m_event) {
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
void MainGLWidget::changeRotateMatrix(QMatrix4x4 &R, float dx, float dy) {
    R.rotate(-dx, 0, 1, 0);         // Умножение R на матрицу поворота вокруг оси y
    R.rotate(-dy, 1, 0, 0);         // Умножение R на матрицу поворота вокруг оси x
}


// Обработчик события прокрутки колеса мыши
void MainGLWidget::wheelEvent(QWheelEvent *w_event) {
    // При прокрутке колеса мыши изменяем глубину объекта
    zoffset -= (float) (w_event->angleDelta().x() + w_event->angleDelta().y()) / 500.0f;
    resetModelView();
    update(); // Перерисовать окно
}


void MainGLWidget::mousePressEvent(QMouseEvent *m_event) {
    mousePosition = m_event->pos();

    // Проверим, какие объекты лежат на пути селектирующего луча и вычислим их глубину (customDepth)
    QVector3D R[2]; // Координаты точек пересечения
    //цикл по всем кубам
    for (int i = 0; i < 2; i++) {
        // Рассчитаем параметры селектирующего луча
        JRay selection_ray = selectionRay(mousePosition, &cubes[i]);
        if (cubes[i].is_selecting = cubes[i].intersects(selection_ray, R) > 0) {
            depthInfo = QString("Глубина точек пересечения луча с кубом %1. Глубина: Z1=%2, Z2=%3.\n").arg(i + 1)
                    .arg(customDepth(R[0], cubes[i].modelViewMatrix))
                    .arg(customDepth(R[1], cubes[i].modelViewMatrix));
        }
    }

    //цикл по всем сферам

    // Рассчитаем параметры селектирующего луча
    JRay selection_ray = selectionRay(mousePosition, &sphere);
    if (sphere.is_selecting = sphere.intersects(selection_ray, R) > 0) {
        depthInfo = QString("Глубина точек пересечения луча с кубом %1. Глубина: Z1=%2, Z2=%3.\n").arg(1)
                .arg(customDepth(R[0], sphere.modelViewMatrix))
                .arg(customDepth(R[1], sphere.modelViewMatrix));
    }


    update();
}

float MainGLWidget::customDepth(const QVector3D &A, QMatrix4x4 modelViewMatrix) {
    // Для определения глубины точки A нужно умножить видовую матрицу на однородные координаты точки A
    QVector4D R = (modelViewMatrix * QVector4D(A.x(), A.y(), A.z(), 1));
    return -R.z(); // Глубина - координата z найденной точки
}

JRay MainGLWidget::selectionRay(const QPoint &P, JCube *cube) const {
    // Вычислим координаты указателя мыши в экранной системе координат OpenGL
    QPointF M = toOpenGLScreen(P);

    // Вычислим параметры селектирующего луча
    // Для этого нужно взять две точки, прямая через которые перпендикулярна экранной плоскости и пересекает её в точке P(x, y)
    // Первая точка A должна находится на ближней плоскости отсечения (z = -1), вторая точка B - на дальней плоскости отсечения (z = 1)
    QVector3D A = (cube->IQ * QVector4D(M.x(), M.y(), -1, 1)).toVector3DAffine();
    QVector3D B = (cube->IQ * QVector4D(M.x(), M.y(), 1, 1)).toVector3DAffine();
    return JRay(A, B);
}

JRay MainGLWidget::selectionRay(const QPoint &P, JSphere *sphere) const {
    // Вычислим координаты указателя мыши в экранной системе координат OpenGL
    QPointF M = toOpenGLScreen(P);

    // Вычислим параметры селектирующего луча
    // Для этого нужно взять две точки, прямая через которые перпендикулярна экранной плоскости и пересекает её в точке P(x, y)
    // Первая точка A должна находится на ближней плоскости отсечения (z = -1), вторая точка B - на дальней плоскости отсечения (z = 1)
    QVector3D A = (sphere->IQ * QVector4D(M.x(), M.y(), -1, 1)).toVector3DAffine();
    QVector3D B = (sphere->IQ * QVector4D(M.x(), M.y(), 1, 1)).toVector3DAffine();
    return JRay(A, B);
}

QPointF MainGLWidget::toOpenGLScreen(QPoint pos) const {
    float mx = -1.0f + 2.0f * (double) pos.x() / width();
    float my = 1.0f - 2.0f * (double) pos.y() / height();
    return QPointF(mx, my);
}

void MainGLWidget::keyPressEvent(QKeyEvent *event) {
    // Закрыть окно при нажатии клавиши Escape
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}
