#include "GYLogModel.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace {
    const int MAX_LINES = 500;
}

GYLogModel::GYLogModel(QObject *parent)
    : QObject(parent)
    , mLines()
    , mMutex()
    , mFileWatcher()
    , mFilePosition(0)
{
    connect(&mFileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));
}

void GYLogModel::watchFile(const QString &path)
{
    qDebug() << "watch file " << path;
    QMutexLocker lock(&mMutex);
    if (!mFileWatcher.files().empty())
    {
        mFileWatcher.removePaths(mFileWatcher.files());
    }
    mFileWatcher.addPath(path);
    mFilePosition = 0;
    mLines.clear();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        mLines.push_back(line);
    }
    mFilePosition = in.pos();
    lock.unlock();

    emit updated();
}

int GYLogModel::lineCount() const
{
    QMutexLocker lock(&mMutex);
    return mLines.size();
}

QString GYLogModel::textAt(int line) const
{
    QMutexLocker lock(&mMutex);
    return mLines[line];
}

void GYLogModel::addLine(const QString &line)
{
    QMutexLocker lock(&mMutex);
    mLines.push_back(line);
    if (mLines.size() > MAX_LINES) {
        mLines.pop_front();
    }
}

void GYLogModel::onFileChanged(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream in(&file);

    QMutexLocker lock(&mMutex);
    if (!in.seek(mFilePosition))
    {
        mFilePosition = 0;
        mLines.clear();
    }
    while (!in.atEnd())
    {
        QString line = in.readLine();
        mLines.push_back(line);
    }
    lock.unlock();

    qDebug() << "onFileChanged " << path;
    emit updated();
}
