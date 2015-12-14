#include "renderarea.h"
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include "DataNavigator/projectionpanel.h"
RenderArea::RenderArea(QWidget *parent)
{
    QRect rect = QApplication::desktop()->screenGeometry();
    width = rect.width() * 0.8;
    height = rect.height() * 0.7;

    this->loader = new  FileLoader("/Users/lizhimin/Desktop/QT/ND_Touch_Pad/iris.csv");
    map = new GrandMap(loader->getDataSource(), 30, 30, width/3-10, width/3-10);
    pcoordinate = new ParallelCoordinate(10, width/3 + 40, width - 20, width/6, map->getProjectionViews());
    scagnostic = new Scagnostic(width/2-20, 20, width/2, width/3, map->getProjectionViews());
    scagnostic->setMap(map);
    info = new InfoPanel(width/2-50, 30, width/2, width/3, map->getXP(), map->getYP(), map->getProjectionDistance());
}

void RenderArea::SamplingMethodChange(int method)
{
    switch(method){
       case 1:
            map->projectionviewGenerator("WE");
            pcoordinate->setProjectionViews(map->getProjectionViews());
        break;
       case 2:
            map->projectionviewGenerator("GT");
            pcoordinate->setProjectionViews(map->getProjectionViews());
        break;
    }
    //change this
    map->Update();

    this->update();
}

void RenderArea::FliteringMethodChange(int method, int k)
{
    switch(method){
        case 0:
            map->Filtering("NC", k);//don't filter and projection view.
        break;
        case 1:
            map->Filtering("K-Mean", k);//use K-Mean to filter projection view
        break;
        case 2:break;
    }
    this->update();
}

void RenderArea::SampeNumberChange(int num)
{
    map->setSampleSize(num);
}


QSize RenderArea::minimumSizeHint() const
{

    return QSize(width, height);
}

QSize RenderArea::sizeHint() const
{
    return QSize(width, height);
}



void RenderArea::paintEvent(QPaintEvent *event)
{
     QPainter* painter = new QPainter(this);
     pcoordinate->paint(painter, NULL, this);
     map->paint(painter,NULL,this);
     //info->paint(painter, NULL, this);
     scagnostic->paint(painter, NULL, this);

}

void RenderArea::mousePressEvent(QMouseEvent *event)
{
    map->mousePressEvent(event);
    pcoordinate->mousePressEvent(event);
    scagnostic->mousePressEvent(event);
    this->update();
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    map->mousePressEvent(event);
    pcoordinate->mouseMoveEvent(event);
    this->update();
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{
    map->mouseReleaseEvent(event);
    pcoordinate->mouseReleaseEvent(event);
    this->update();
}
