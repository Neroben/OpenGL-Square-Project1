
#include <QApplication>
#include "OpenGLWidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);


    OpenGLWidget w(nullptr);
    w.show();

    return app.exec();
}
