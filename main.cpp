
#include "SquareWindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    SquareWindow window;
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return QGuiApplication::exec();
}
