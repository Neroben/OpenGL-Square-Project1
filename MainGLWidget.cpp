// Автор: Осипов Олег Васильевич
// Copyright © 2019 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем. Все права защищены.

#include "MainGLWidget.h"
#include <gl/glu.h>

MainGLWidget::MainGLWidget(QWidget *parent)
        : QOpenGLWidget(parent)
{
    setWindowTitle("Пересечение примитивов с селектирующим лучом");
    setWindowState(Qt::WindowFullScreen);
}

void MainGLWidget::initializeGL()
{
    // Включение сортировки по глубине
    glEnable(GL_DEPTH_TEST);

    // Режим рисования только лицевых граней
    glEnable(GL_CULL_FACE);

    // Задаём такой режим смешивания цветов, чтобы работала прозрачность
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Инициализация освещения
    initLighting();

    // Инициализация матрицы поворота в виде единичной матрицы
    rotateMatrix.setToIdentity();

    // Создание 7 кубов
    QVector3D V1 = QVector3D(-1, -1, -1);
    QVector3D V2 = QVector3D( 1,  1,  1);
    cubes[0].init(V1, V2);

    QVector3D H = QVector3D(0, 0, 3);
    cubes[1].init(V1 + H, V2 + H);
    cubes[2].init(V1 - H, V2 - H);

    H = QVector3D(0, 3, 0);
    cubes[3].init(V1 + H, V2 + H);
    cubes[4].init(V1 - H, V2 - H);

    H = QVector3D(3, 0, 0);
    cubes[5].init(V1 + H, V2 + H);
    cubes[6].init(V1 - H, V2 - H);

    // Создание 4 сфер
    spheres[0] = JSphere(0, 6, 0, 1);
    spheres[1] = JSphere(3, 3, 0, 1);
    spheres[2] = JSphere(6, 0, 0, 1);
    spheres[3] = JSphere(9, -3, 0, 1);
}


void MainGLWidget::initLighting()
{
    // Включение освещения
    glEnable(GL_LIGHTING);

    // Включение автоматической нормализации векторов нормалей (обязательно при использовании операций масштабирования)
    glEnable(GL_NORMALIZE);

    // Освещение обеих сторон полигонов
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    // Параметры направленного источника (как солнце)
    float light0_diffuse[] = {0.9, 0.7, 0.1}; // Цвет
    float light0_ambient[] = {0.0, 0.0, 0.4, 1.0};
    // У направленного источника 4-ая компонента w = 0, а первые три имеют обратный знак
    float light0_direction[] = {1.0, 0.0, 0.0, 0.0}; // Направление: (-1, 0, 0)
    glEnable(GL_LIGHT0); // Включение источника
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);    // Задаём цвет
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction); // Задаём направление
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);    // Цвет рассеянного освещения

    // Параметры точечного источника (как лампа)
    float light1_diffuse[] = {0.4, 0.9, 0.4}; // Цвет

    // У точечного источника 4-ая компонента w = 1
    float light1_position[] = {0.0, 0.0, -1.0, 1.0}; // Координаты центра источника света
    glEnable(GL_LIGHT1); // Включение источника
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);  // Задаём цвет
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position); // Задаём положение

    // Задаём свойства материала
    float specular_color[] = {1.0, 1.0, 1.0, 1}; // Цвет бликов
    float shininess[] = { 30 };
    // Цвет отражённого света
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_color);
    // Степень отраженного света
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

}


void MainGLWidget::resizeGL(int nWidth, int nHeight)
{
    // Задание области вывода
    glViewport(0, 0, nWidth, nHeight);

    // Обновляем матрицу центрального проектирования
    resetProjectionMatrix();

    // Обновляем видовую матрицу
    resetModelViewMatrix();
}



void MainGLWidget::paintGL()
{
    // Очистка буфера глубины и буфера цвета
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Рисование кубов
    for (int i=0; i < 7; i++)
    {
        // Изменяем цвет куба в зависимости от того, выделен он или нет
        if (cubes[i].is_selecting) // Если выделен
        {
            float color[] = { 0.0, 162/255.0, 232/255.0, 1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
        }
        else // Если не выделен
        {
            float color[] = { i * 0.1f, 0.3, 1 - i * 0.1f, 1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
        }

        // Рисование куба
        drawCube(cubes[i]);
    }

    // Рисование сфер
    GLUquadric* sphereObj = gluNewQuadric();
    for (int i=0; i < 4; i++)
    {
        JSphere S = spheres[i];

        // Изменяем цвет сферы в зависимости от того, выделена она или нет
        if (S.is_selecting) // Если выделена
        {
            float color[] = { 0.0, 162/255.0, 232/255.0, 1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
        }
        else // Если не выделена
        {
            float color[] = { i * 0.1f, 0.3, 1 - i * 0.1f, 1};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
        }

        glPushMatrix();
        glTranslatef(S.x0(), S.y0(), S.z0());
        gluSphere(sphereObj, 1, 100, 100);
        glPopMatrix();
    }
    gluDeleteQuadric(sphereObj);

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


void MainGLWidget::drawCube(const JCube &cube)
{
    QVector3D N, V;
    glBegin(GL_QUADS);
    // Проходим по всем граням куба
    for (int i=0; i < cube.polygons.size(); i++)
    {
        JPolygon P = cube.polygons[i];
        // Задаём вектор нормали к грани
        N = P.normal();
        glNormal3f(N.x(), N.y(), N.z());
        // Перечисляем координаты вершин
        for (int j=0; j < P.nVertices();j++)
        {
            V = P[j];
            glVertex3f(V.x(), V.y(), V.z());
        }
    }
    glEnd();
}



void MainGLWidget::resetProjectionMatrix()
{
    // Переключение на матрицу проектирования
    glMatrixMode(GL_PROJECTION);
    // Загрузка единичной матрицы проектирования
    glLoadIdentity();
    // Инициализация перспективной проекции
    gluPerspective(30.0, (float)width() / height(), 0.1, 20);
}


void MainGLWidget::resetModelViewMatrix()
{
    // Переключение на видовую матрицу
    glMatrixMode(GL_MODELVIEW);
    // Инициализация видовой матрицы в виде единичной матрицы
    glLoadIdentity();
    // Перенос вглубь экрана
    glTranslatef(0, 0, -zoffset);
    // Поворот объекта
    glMultMatrixf(rotateMatrix.data());
    // Масштабирование
    glScalef(0.3, 0.3, 0.3);

    // Получим текущие матрицы OpenGL для проектирования и аффинных преобразований
    QMatrix4x4 projectionMatrix, modelViewMatrix;
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix.data());
    glGetFloatv(GL_MODELVIEW_MATRIX,  modelViewMatrix.data());

    // Умножим матрицы проектирования и видовую матрицы
    Q = projectionMatrix * modelViewMatrix;

    // Вычислим обратную матрицу
    IQ = Q.inverted();
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
    // Обновим видовую матрицу
    resetModelViewMatrix();
    update(); // Перерисовать окно
}



// Процедура предназначена для изменения матрицы поворота, чтобы объекты поворачивалась в нужном направлении строго вслед за указателем мыши.
// Вызывается, когда пользователь изменил положение указателя мыши при зажатой кнопке (мыши)
void MainGLWidget::changeRotateMatrix(QMatrix4x4& rotateMatrix, float dx, float dy)
{
    float* data = rotateMatrix.data();
    QMatrix4x4 R(data);             // Создание матрицы R из вещественного массива длиной 16
    R.rotate(-dx, 0, 1, 0);         // Умножение R на матрицу поворота вокруг оси y
    R.rotate(-dy, 1, 0, 0);         // Умножение R на матрицу поворота вокруг оси x
    R.copyDataTo(data);             // Копирование матрицы R в исходный массив

    // То же самое можно сделать, транспонируя матрицы:
    /*QMatrix4x4 R = rotateMatrix.transposed();
    R.rotate(-dx, 0, 1, 0);         // Умножение R на матрицу поворота вокруг оси y
    R.rotate(-dy, 1, 0, 0);         // Умножение R на матрицу поворота вокруг оси x
    rotateMatrix = R.transposed();*/
}


// Обработчик события прокрутки колеса мыши
void MainGLWidget::wheelEvent(QWheelEvent* w_event)
{
    // При прокрутке колеса мыши изменяем глубину объекта
    zoffset -= (float)(w_event->angleDelta().x() + w_event->angleDelta().y()) / 500.0f;
    resetModelViewMatrix(); // Обновим видовую матрицу
    update(); // Перерисовать окно
}



float MainGLWidget::depth(QVector3D A)
{
    QMatrix4x4 M;
    // Для определения глубины точки A нужно умножить видовую матрицу на однородные координаты точки A
    glGetFloatv(GL_MODELVIEW_MATRIX,  M.data());
    QVector4D R = (M * QVector4D(A.x(), A.y(), A.z(), 1));
    return -R.z(); // Глубина - координата z найденной точки
}

// То же самое можно сделать и так:
/*float MainGLWidget::depth(QVector3D A)
{
    float M[16];
    glGetFloatv(GL_MODELVIEW_MATRIX,  M);
    // Для определения глубины точки A нужно умножить третью строку видовой матрицы на однородные координаты точки A
    return -(M[2]*A.x() + M[6]*A.y() + M[10]*A.z() + M[14]);
}*/



QPointF MainGLWidget::toOpenGLScreen(QPoint pos) const
{
    float mx = -1.0f + 2.0f * (double) pos.x() / width();
    float my =  1.0f - 2.0f * (double) pos.y() / height();
    return QPointF(mx, my);
}



JRay MainGLWidget::selectionRay(const QPoint& P) const
{
    // Вычислим координаты указателя мыши в экранной системе координат OpenGL
    QPointF M = toOpenGLScreen(P);

    // Вычислим параметры селектирующего луча
    // Для этого нужно взять две точки, прямая через которые перпендикулярна экранной плоскости и пересекает её в точке P(x, y)
    // Первая точка A должна находится на ближней плоскости отсечения (z = -1), вторая точка B - на дальней плоскости отсечения (z = 1)
    QVector3D A = (IQ * QVector4D(M.x(), M.y(), -1, 1)).toVector3DAffine();
    QVector3D B = (IQ * QVector4D(M.x(), M.y(),  1, 1)).toVector3DAffine();
    return JRay(A, B);
}



// Второй вариант реализации данной функции
/*JRay MainGLWidget::selectionRay(const QPoint &P) const
{
    // Матрица аффинных преобразований и матрица проектирования
    QMatrix4x4 modelViewMatrix, projectionMatrix;
    // Параметры области вывода. Первые 2 параметра - координаты левого нижнего угла в оконной системе координат (Linux или Windows).
    // Третий параметр - ширина окна.
    // 4-ый - высота окна (с обратным знаком, так как ось Y в экранной системе OpenGL направлена вверх, а в оконной системе вниз)
    QRect viewport(0, height(), width(), -height());
    // Извлечём текущие матрицу проектирования и видовую матрицу
    glGetFloatv(GL_MODELVIEW_MATRIX,  modelViewMatrix.data());
    glGetFloatv(GL_PROJECTION_MATRIX,  projectionMatrix.data());
    // Функция unproject выполняет операцию, обратную операции проектирования, т.е. переводит экранные координаты
    // (P.x, P.y, -1), (P.x, P.y, 1), заданные в пикселях, в мировые по заданным матрицам проектирования, вида и параметрам области вывода.
    QVector3D A = QVector3D(P.x(), P.y(), -1).unproject(modelViewMatrix, projectionMatrix, viewport);
    QVector3D B = QVector3D(P.x(), P.y(),  1).unproject(modelViewMatrix, projectionMatrix, viewport);
    return JRay(A, B);
}*/



// Третий вариант реализации данной функции
/*JRay MainGLWidget::selectionRay(const QPoint &P) const
{
    // Параметры области вывода
    GLint  viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    // Матрица аффинных преобразований и матрица проектирования
    double modelViewMatrix[16], projectionMatrix[16];
    // Извлечём текущие матрицу проектирования и видовую матрицу
    glGetDoublev(GL_MODELVIEW_MATRIX,  modelViewMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX,  projectionMatrix);
    double x, y, z;
    // Расчёт мировых координат точки пересечения селектирующего луча с ближней плоскостью отсечения
    gluUnProject(P.x(), height() - P.y(), -1, modelViewMatrix, projectionMatrix, viewport, &x, &y, &z);
    QVector3D A = QVector3D(x, y, z);
    // Вторая точка, лежащая на дальней плоскости отсечения, для построения селектирующего луча
    gluUnProject(P.x(), height() - P.y(),  1, modelViewMatrix, projectionMatrix, viewport, &x, &y, &z);
    QVector3D B = QVector3D(x, y, z);
    return JRay(A, B);
}*/



void MainGLWidget::mousePressEvent(QMouseEvent* m_event)
{
    mousePosition = m_event->pos();

    // Рассчитаем параметры селектирующего луча
    JRay selection_Ray = selectionRay(mousePosition);

    // Проверим, какие объекты лежат на пути селектирующего луча и вычислим их глубину (depth)
    output_text = "";
    QVector3D R[2]; // Координаты точек пересечения
    for (int i=0; i < 7;i++)
    {
        if (cubes[i].is_selecting = cubes[i].intersects(selection_Ray, R) > 0)
        {
            output_text = output_text + QString("Куб %1. Глубины: Z1=%2, Z2=%3.\n").arg(i+1).arg(depth(R[0])).arg(depth(R[1]));
        }
    }
    for (int i=0; i < 4; i++)
    {
        if(spheres[i].is_selecting = spheres[i].intersects(selection_Ray, R) > 0)
        {
            output_text = output_text + QString("Сфера %1. Глубины: Z1=%2, Z2=%3.\n").arg(i+1).arg(depth(R[0])).arg(depth(R[1]));
        }
    }
    update(); // Перерисовать окно
}


void MainGLWidget::keyPressEvent(QKeyEvent* event)
{
    // Закрыть окно при нажатии клавиши Escape
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
}