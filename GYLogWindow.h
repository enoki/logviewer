#pragma once

#include <QtWidgets/QMainWindow>

class GYLogModel;

class GYLogWindow : public QMainWindow
{
    Q_OBJECT
public:
    GYLogWindow(QWidget *parent=nullptr);

private slots:
    void fileOpen();

private:
    void openFile(const QString &path);

    void loadSettings();
    void saveSettings();

private:
    GYLogModel *mModel;
    QString mLastDir;
};
