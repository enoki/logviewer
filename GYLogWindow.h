#pragma once

#include <QtWidgets/QWidget>

class GYLogModel;

class GYLogWindow : public QWidget
{
public:
    GYLogWindow(QWidget *parent=nullptr);

private:
    GYLogModel *mModel;
};
