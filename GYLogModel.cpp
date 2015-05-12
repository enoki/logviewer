#include "GYLogModel.h"
#include <QFile>
#include <QFileInfo>
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
    , mUpdateTimer()
    , mPollTimer()
    , mFileSize(0)
{
    connect(&mFileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));

    mUpdateTimer.setSingleShot(true);
    mUpdateTimer.setInterval(250);
    connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(update()));

    mPollTimer.setInterval(500);
    connect(&mPollTimer, SIGNAL(timeout()), this, SLOT(onPollTimer()));
}

void GYLogModel::watchFile(const QString &path)
{
    qDebug() << "watch file " << path;
    QMutexLocker lock(&mMutex);
    mPollTimer.stop();
    mPath = path;
    if (!mFileWatcher.files().empty())
    {
        mFileWatcher.removePaths(mFileWatcher.files());
    }
    if (!mFileWatcher.addPath(path))
    {
        qCritical() << "Unable to watch file: " << path;
    }
    mFilePosition = 0;
    mFileSize = 0;
    mLines.clear();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file: " << path;
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        mLines.push_back(line);
    }
    mFilePosition = in.pos();
    mFileSize = mFilePosition;
    lock.unlock();

    emit updated();
    mPollTimer.start();
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

void GYLogModel::scheduleUpdate()
{
    if (mUpdateTimer.isActive())
    {
        return;
    }
    mUpdateTimer.start();
}

void GYLogModel::onFileChanged(const QString &path)
{
    scheduleUpdate();
}

void GYLogModel::onPollTimer()
{
    if (QFileInfo(mPath).size() != mFileSize)
    {
        qDebug() << "Size changed, updating";
        scheduleUpdate();
    }
}

void GYLogModel::update()
{
    QString path = mPath;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file: " << path;
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
    mFilePosition = in.pos();
    mFileSize = mFilePosition;
    lock.unlock();

    qDebug() << "onFileChanged " << path;
    emit updated();
}
