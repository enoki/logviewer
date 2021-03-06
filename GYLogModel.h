#pragma once

#include <vector>
#include <QMutex>
#include <QString>
#include <QFileSystemWatcher>
#include <QTimer>

class QString;

class GYLogModel : public QObject
{
    Q_OBJECT
public:
    GYLogModel(QObject *parent=nullptr);

    void watchFile(const QString &path);

    int lineCount() const;
    QString textAt(int line) const;

signals:
    void updated();

private slots:
    void onFileChanged(const QString &path);
    void onPollTimer();
    void update();

private:
    void scheduleUpdate();

private:
    std::vector<QString> mLines;
    mutable QMutex mMutex;
    QFileSystemWatcher mFileWatcher;
    qint64 mFilePosition;
    qint64 mFileSize;
    QString mPath;
    QTimer mUpdateTimer;
    QTimer mPollTimer;
};
