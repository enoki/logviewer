#pragma once

#include <deque>
#include <QMutex>
#include <QString>
#include <QFileSystemWatcher>

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

private:
    void addLine(const QString &line);

private:
    std::deque<QString> mLines;
    mutable QMutex mMutex;
    QFileSystemWatcher mFileWatcher;
    qint64 mFilePosition;
};
