#include "GYLogWindow.h"
#include "GYLogView.h"
#include "GYLogModel.h"
#include <QtWidgets/QVBoxLayout>

GYLogWindow::GYLogWindow(QWidget *parent)
    : QWidget(parent)
    , mModel(new GYLogModel(this))
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new GYLogView(mModel));
    setLayout(layout);

    mModel->watchFile("/Users/gyoshida/Library/Logs/Lytro/log.txt");
}
