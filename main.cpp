
#include <QApplication>
#include "MainGLWidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainGLWidget w(nullptr);
    w.show();

    return app.exec();
}
