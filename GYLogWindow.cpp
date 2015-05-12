#include "GYLogWindow.h"
#include "GYLogView.h"
#include "GYLogModel.h"
#include <QtWidgets/QVBoxLayout>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QStandardPaths>

namespace
{
    static QString settingsDir()
    {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    }
    static QString settingsPath()
    {
        return settingsDir() + "/preferences.ini";
    }
}

GYLogWindow::GYLogWindow(QWidget *parent)
    : QMainWindow(parent)
    , mModel(new GYLogModel(this))
    , mLastDir()
{
    setCentralWidget(new GYLogView(mModel));

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Open..."), this, SLOT(fileOpen()), tr("Ctrl+O"));

    loadSettings();
    if (!mLastDir.isEmpty())
    {
        openFile(mLastDir);
    }
}

void GYLogWindow::fileOpen()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), mLastDir,
        tr("Log Files (*.txt *.log);;All Files (*.*)"));
    if (path.isEmpty())
    {
        return;
    }
    openFile(path);
    mLastDir = path;
    saveSettings();
}

void GYLogWindow::openFile(const QString &path)
{
    mModel->watchFile(path);
}

void GYLogWindow::loadSettings()
{
    QString path(settingsPath());
    qDebug() << "Settings: " << path;
    QSettings settings(path, QSettings::IniFormat);
    mLastDir = settings.value("lastOpenedFile", QDir::homePath()).toString();
}

void GYLogWindow::saveSettings()
{
    if (!QDir().mkpath(settingsDir()))
    {
        qCritical() << "Unable to create settings dir: " << settingsDir();
    }
    QSettings settings(settingsPath(), QSettings::IniFormat);
    settings.setValue("lastOpenedFile", mLastDir);
}
