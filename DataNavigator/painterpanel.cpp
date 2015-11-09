#include "painterpanel.h"
#include <QDesktopWidget>
#include <QApplication>
PainterPanel::PainterPanel(Storage *storage, ProjectionView *p, QWidget *parent) : QWidget(parent)
{
    QRect rect = QApplication::desktop()->screenGeometry();
    width = rect.width() * 0.5;
    height = rect.height() * 0.7;
    this->resize(rect.width()/2, rect.height());
    this->plot = new ScatterPlot(storage,20, 20, width/2, width/2, p);
    this->pad = new NDPad(width/2 + width/2, width/4, storage->getLabels());
    this->pad->setPlot(plot);
    this->pad->setInitalProjection(p->getXP(), p->getYP());
    this->info = new ScatterPlotInfoPanel(10, width/2 + 50, width - 20, height/3,
                                          storage->getLabels(), p->getXP(), p->getYP());
}

PainterPanel::~PainterPanel()
{

}

void PainterPanel::setProjectionView(ProjectionView *p)
{
    plot->setProjectionView(p);
    pad->setInitalProjection(p->getXP(), p->getYP());
    this->update();
}

QSize PainterPanel::minimumSizeHint() const
{
    return QSize(width, height);
}

QSize PainterPanel::sizeHint() const
{
    return QSize(width, height);
}

void PainterPanel::paintEvent(QPaintEvent *event)
{
    QPainter* painter = new QPainter(this);
    plot->paint(painter, NULL, this);
    pad->paint(painter, NULL, this);
    info->paint(painter, NULL, this);
}

void PainterPanel::mousePressEvent(QMouseEvent *event)
{
    pad->mousePressEvent(event);
    this->update();
}

void PainterPanel::mouseMoveEvent(QMouseEvent *event)
{
    pad->mouseMoveEvent(event);
    this->update();
}

