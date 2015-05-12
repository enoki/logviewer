#pragma once

#include <QtWidgets/QAbstractScrollArea>
#include <QtGui/QPen>

class GYLogModel;

class GYLogView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    GYLogView(GYLogModel *model, QWidget *parent=nullptr);

signals:
    void internalScheduleUpdate();

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void scrollContentsBy(int dx, int dy) override;
    void setupViewport(QWidget *viewport) override;
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void onUpdated();
    void doUpdate();
    void scheduleUpdate();

private:
    GYLogModel *mModel;
    int mLineHeight;
    int mLinesPerScreen;
    int mLineBaseline;
    QTimer *mTimer;
    QPen mPen;
};
