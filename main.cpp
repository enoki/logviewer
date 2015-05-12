#include <QApplication>
#include "GYLogWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Enoki");
    app.setOrganizationDomain("enoki.github.com");
    app.setApplicationName("LogViewer");

    GYLogWindow *win = new GYLogWindow();
    win->show();
    return app.exec();
}
