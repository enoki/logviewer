#include <QApplication>
#include "GYLogWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GYLogWindow *win = new GYLogWindow();
    win->show();
    return app.exec();
}
