#include "GYLogView.h"
#include "GYLogModel.h"
#include <QPainter>
#include <QScrollBar>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>

GYLogView::GYLogView(GYLogModel *model, QWidget *parent)
    : QAbstractScrollArea(parent)
    , mLineHeight(0)
    , mLinesPerScreen(0)
    , mLineBaseline(0)
    , mModel(model)
    , mTimer(new QTimer(this))
    , mPen()
{
    QFontMetrics metrics(font());
    mLineHeight = metrics.height();
    mLineBaseline = metrics.ascent();

    verticalScrollBar()->setPageStep(1);
    verticalScrollBar()->setSingleStep(1);

    connect(mModel, SIGNAL(updated()), this, SLOT(onUpdated()));
    connect(this, SIGNAL(internalScheduleUpdate()), this, SLOT(scheduleUpdate()));

    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);
    mTimer->setInterval(250);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(doUpdate()));
}

void GYLogView::paintEvent(QPaintEvent *)
{
    QPainter painter(viewport());
    painter.setPen(mPen);

    int firstLine = verticalScrollBar()->value();
    int lastLine = firstLine + mLinesPerScreen;
    int y = 0;
    //qDebug() << "paint " << firstLine << "," << lastLine;

    if (lastLine > mModel->lineCount()) {
        lastLine = mModel->lineCount();
    }

    for (int line = firstLine; line < lastLine; ++line) {
        QString text = mModel->textAt(line);

        text = text.mid(horizontalScrollBar()->value());

        painter.drawText(mLineHeight+10, y + mLineBaseline, text);

        y += mLineHeight;
    }
}

void GYLogView::resizeEvent(QResizeEvent *event)
{
    mLinesPerScreen = viewport()->height() / mLineHeight;
    QAbstractScrollArea::resizeEvent(event);
    qDebug() << "lines per=" << mLinesPerScreen;
}

//void GYLogView::scrollContentsBy(int /*dx*/, int /*dy*/)
void GYLogView::scrollContentsBy(int dx, int dy)
{
    //QTimer::singleShot(0, this, SLOT(update()));
    //qDebug() << "scrollContentsBy (" << dx << "," << dy << ")";
    //qDebug() << "updatesEnabled=" << updatesEnabled();
    //update();
    QAbstractScrollArea::scrollContentsBy(dx, dy);
}

void GYLogView::setupViewport(QWidget *viewport)
{
    QAbstractScrollArea::setupViewport(viewport);
}

void GYLogView::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_G:
        {
            if (e->modifiers() & Qt::ShiftModifier)
            {
                verticalScrollBar()->setValue(verticalScrollBar()->maximum());
            }
            else
            {
                verticalScrollBar()->setValue(verticalScrollBar()->minimum());
            }
            break;
        }
        case Qt::Key_J:
        {
            verticalScrollBar()->setValue(verticalScrollBar()->value()+1);
            break;
        }
        case Qt::Key_K:
        {
            verticalScrollBar()->setValue(verticalScrollBar()->value()-1);
            break;
        }
        case Qt::Key_H:
        {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value()-1);
            break;
        }
        case Qt::Key_L:
        {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value()+1);
            break;
        }
        case Qt::Key_Space:
        {
            verticalScrollBar()->setValue(verticalScrollBar()->value()+mLinesPerScreen);
            break;
        }
        default:
        {
            QAbstractScrollArea::keyPressEvent(e);
            break;
        }
    }
}

void GYLogView::onUpdated()
{
    emit internalScheduleUpdate();
}

void GYLogView::doUpdate()
{
    int lineCount = mModel->lineCount();
    verticalScrollBar()->setPageStep(mLinesPerScreen);

    int rangeMax = lineCount - mLinesPerScreen;
    if (rangeMax < 0) {
        rangeMax = 0;
    }
    verticalScrollBar()->setRange(0, rangeMax);
    verticalScrollBar()->setValue(rangeMax);

    horizontalScrollBar()->setRange(0, 128);
    horizontalScrollBar()->setValue(0);

    update();
    qDebug() << "onUpdate: " << lineCount << "," << mLinesPerScreen << "," << rangeMax;
}

void GYLogView::scheduleUpdate()
{
    if (mTimer->isActive()) {
        return;
    }
    mTimer->start();
}
