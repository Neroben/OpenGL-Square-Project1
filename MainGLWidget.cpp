// Автор: Осипов Олег Васильевич
// Copyright © 2020 БГТУ им. В.Г. Шухова. Кафедра программного обеспечения вычислительной техники и автоматизированных систем.
// Дата изменения: 15.03.2020

// Приложение, использующее простейший шейдер для освещения куба

#include "MainGLWidget.h"
#include "geometry/JCube.h"

void MainGLWidget::initializeGL() {
    // Включение сортировки по глубине
    glEnable(GL_DEPTH_TEST);

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

    // Цвет объекта
    shaderProgram.setUniformValue("objectColor", QVector3D(0.0, 0.5, 0.9));

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
void MainGLWidget::glCube() {
    // Массив из 24 вершин (6 граней куба, каждая из 4 вершин)
    static const float vertices[6 * 4][3] =
            {{-1.0f, 1.0f,  1.0f},
             {-1.0f, -1.0f, 1.0f},
             {1.0f,  -1.0f, 1.0f},
             {1.0f,  1.0f,  1.0f},  // Передняя грань (z =  1)
             {1.0f,  1.0f,  -1.0f},
             {1.0f,  -1.0f, -1.0f},
             {-1.0f, -1.0f, -1.0f},
             {-1.0f, 1.0f,  -1.0f},  // Задняя грань   (z = -1)
             {1.0f,  1.0f,  -1.0f},
             {-1.0f, 1.0f,  -1.0f},
             {-1.0f, 1.0f,  1.0f},
             {1.0f,  1.0f,  1.0f},  // Верхняя грань  (y =  1)
             {1.0f,  -1.0f, 1.0f},
             {-1.0f, -1.0f, 1.0f},
             {-1.0f, -1.0f, -1.0f},
             {1.0f,  -1.0f, -1.0f},  // Нижняя грань   (y = -1)
             {1.0f,  -1.0f, 1.0f},
             {1.0f,  -1.0f, -1.0f},
             {1.0f,  1.0f,  -1.0f},
             {1.0f,  1.0f,  1.0f},  // Правая грань   (x =  1)
             {-1.0f, 1.0f,  1.0f},
             {-1.0f, 1.0f,  -1.0f},
             {-1.0f, -1.0f, -1.0f},
             {-1.0f, -1.0f, 1.0f}}; // Левая грань    (x = -1)

    // Массив нормалей каждой вершины из массива vertices
    static const float normals[6 * 4][3] =
            {{0.0f,  0.0f,  1.0f},
             {0.0f,  0.0f,  1.0f},
             {0.0f,  0.0f,  1.0f},
             {0.0f,  0.0f,  1.0f},   // Вектора нормалей передней грани
             {0.0f,  0.0f,  -1.0f},
             {0.0f,  0.0f,  -1.0f},
             {0.0f,  0.0f,  -1.0f},
             {0.0f,  0.0f,  -1.0f},   // Вектора нормалей задней грани
             {0.0f,  1.0f,  0.0f},
             {0.0f,  1.0f,  0.0f},
             {0.0f,  1.0f,  0.0f},
             {0.0f,  1.0f,  0.0f},   // Вектора нормалей верхней грани
             {0.0f,  -1.0f, 0.0f},
             {0.0f,  -1.0f, 0.0f},
             {0.0f,  -1.0f, 0.0f},
             {0.0f,  -1.0f, 0.0f},   // Вектора нормалей нижней грани
             {1.0f,  0.0f,  0.0f},
             {1.0f,  0.0f,  0.0f},
             {1.0f,  0.0f,  0.0f},
             {1.0f,  0.0f,  0.0f},   // Вектора нормалей правой грани
             {-1.0f, 0.0f,  0.0f},
             {-1.0f, 0.0f,  0.0f},
             {-1.0f, 0.0f,  0.0f},
             {-1.0f, 0.0f,  0.0f}}; // Вектора нормалей левой грани

    QVector3D vec1;
    QVector3D vec2;

    vec1.setX(-1.0f);
    vec1.setY(-1.0f);
    vec1.setZ(-1.0f);
    vec2.setX(1.0f);
    vec2.setY(1.0f);
    vec2.setZ(1.0f);

    JCube cube;
    cube.init(vec1, vec2);

    float *vert = cube.getVertices();

    for (int i = 0; i < 6; i++) {
        QString string;
        for (int j = 0; j < 4; j++) {
            string.append("{");
            for (int k = 0; k < 3; k++) {
                string.append(QString::number(vert[i*4*3 + j * 3 + k]));
                string.append(",");
            }
            string.append("},");
        }
        qDebug() << string;
    }

    qDebug() << "";

    shaderProgram.bind();

    // Устанавливаем параметры освещения
    setLighting();

    // Зададим матрицу, на которую будут умножены однородные координаты вершин в вершинном шейдере
    shaderProgram.setUniformValue(matrixLocation, projectMatrix * modelViewMatrix);

    // Матрица, используемая для пересчёта векторов нормалей в вершинном шейдере
    shaderProgram.setUniformValue(normalMatrixLocation, modelViewMatrix.normalMatrix());

    // Видовая матрица для вершинного шейдера
    shaderProgram.setUniformValue(modelViewMatrixLocation, modelViewMatrix);

    // Передаём массив вершин (координаты каждой вершины задаются тремя числами)
    shaderProgram.setAttributeArray(vertexLocation, cube.getVertices(), 3);

    // Передаём массив векторов нормалей к вершинам vertices. Третий параметр означает, что каждый вектор состоит из трёх чисел
    shaderProgram.setAttributeArray(normalLocation, cube.getNormales(), 3);

    shaderProgram.enableAttributeArray(vertexLocation);

    shaderProgram.enableAttributeArray(normalLocation);

    // Рисование 6 граней куба, координаты и нормали которых заранее переданы в массивах. Всего массивы содержит 24 вершины
    glDrawArrays(GL_QUADS, 0, 6*4);

    shaderProgram.disableAttributeArray(vertexLocation);

    shaderProgram.disableAttributeArray(normalLocation);

    shaderProgram.release();
}


void MainGLWidget::resetProjection() {
    // Инициализация единичной матрицы
    projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    projectMatrix.perspective(30.0, (float) width() / height(), 0.1, 20);
}


// Процедура для изменения видовой матрицы
void MainGLWidget::resetModelView() {
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
    resetModelView(); // Обновим матрицу аффинных преобразований
    update(); // Перерисовать окно
}


void MainGLWidget::mousePressEvent(QMouseEvent *m_event) {
    mousePosition = m_event->pos();
}


void MainGLWidget::keyPressEvent(QKeyEvent *event) {
    // Закрыть окно при нажатии клавиши Escape
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}
