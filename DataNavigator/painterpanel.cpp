#include "painterpanel.h"
#include <QDesktopWidget>
#include <QApplication>
PainterPanel::PainterPanel(Storage *storage, ProjectionView *p, QWidget *parent) : QWidget(parent)
{
    QRect rect = QApplication::desktop()->screenGeometry();
    width = rect.width() * 0.5;
    height = rect.height() * 0.7;
    this->resize(rect.width()/2, rect.height());
    this->plot = new ScatterPlot(storage,30, 30, width/2, width/2, p);
    this->pad = new NDPad(width/2 + width/2, width/4, storage->getLabels(), storage);
    this->info = new ScatterPlotInfoPanel(20, width/2 + 60, width*1.2, height/3 - 30,
                                          storage->getLabels(), p->getXP(), p->getYP());
    this->pad->setPlot(plot);
    this->pad->setInfo(info);
    this->pad->setInitalProjection(p->getXP(), p->getYP());

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

void PainterPanel::attributeChange(int index)
{
    plot->attributeChange(index);
    this->update();
}

void PainterPanel::buttonClick()
{
    pad->resetPad();
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
    plot->mousePressEvent(event);
    this->update();
}

void PainterPanel::mouseMoveEvent(QMouseEvent *event)
{
    pad->mouseMoveEvent(event);
    plot->mouseMoveEvent(event);
    this->update();
}

void PainterPanel::mouseReleaseEvent(QMouseEvent *event)
{
    plot->mouseReleaseEvent(event);
    this->update();
}

