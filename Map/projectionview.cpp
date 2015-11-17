#include "projectionview.h"
#include <QPainter>
#include <math.h>
#include <QDebug>
ProjectionView::ProjectionView(std::vector<float> *xp, std::vector<float> *yp, Storage *storage)
{
    this->xp = xp;
    this->yp = yp;
    this->datasource = storage;
    this->metrics = new DataExplorerHD::GraphTheoreticScagnosticsMetric();
    this->metricsCalculator();
    this->setColor(-1);
    this->width = 10;
    this->height = 10;
    this->visible = true;//in defualt, each projection view will be visible.
}

ProjectionView::~ProjectionView()
{

}

void ProjectionView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(visible){
        QBrush brush(this->color);
        painter->setBrush(brush);
        painter->drawEllipse(this->locationX, this->locationY,this->width, this->height);
    }
}

QRectF ProjectionView::boundingRect() const
{

}

std::vector<float> *ProjectionView::getXP()
{
    return this->xp;
}

std::vector<float> *ProjectionView::getYP()
{
    return this->yp;
}

float ProjectionView::getMetricsValue(int index)
{
    if(index > 8){
        qDebug() << "metricx index out of bound";
        exit(0);
    }else{
        return this->metricsvalue[index];
    }
}

float ProjectionView::getOutlying()
{
    return this->outlying;
}

float ProjectionView::getSkewed()
{
    return this->Skewed;
}

float ProjectionView::getClumpy()
{
    return this->clumpy;
}

float ProjectionView::getSparse()
{
    return this->sparse;
}

float ProjectionView::getStriated()
{
    return this->striated;
}

float ProjectionView::getConvex()
{
    return this->convex;
}

float ProjectionView::getSkinny()
{
    return this->skinny;
}

float ProjectionView::getStringy()
{
    return this->stringy;
}

float ProjectionView::getMonotonic()
{
    return this->monotonic;
}

float ProjectionView::getX()
{
    return this->x;
}

float ProjectionView::getY()
{
    return this->y;
}

QColor ProjectionView::getColor()
{
    return this->color;
}

float ProjectionView::getLocationX()
{
    return this->locationX;
}

float ProjectionView::getLocationY()
{
    return this->locationY;
}

bool ProjectionView::getVisible()
{
    return this->visible;
}

void ProjectionView::setVisible(bool v)
{
    this->visible = v;
}

void ProjectionView::setX(float x)
{
    this->x = x;
}

void ProjectionView::setY(float y)
{
    this->y = y;
}

void ProjectionView::setColor(int color)
{
    switch(color){
    case 0:
        this->color.setRgb(0, 102, 255);
        break;
    case 1:
        this->color.setRgb(0, 204, 0);
        break;
    case 2:
        this->color.setRgb(204, 153, 0);
        break;
    case 3:
        this->color.setRgb(204, 51, 0);
        break;
    case 4:
        this->color.setRgb(0, 102, 102);
        break;
    case 5:
        this->color.setRgb(102, 0, 204);
        break;
    case 6:
        this->color.setRgb(153, 102, 51);
        break;
    case 7:
        this->color.setRgb(255, 153, 0);
        break;
    case 8:
        this->color.setRgb(102, 102, 155);
        break;
    case 9:
        this->color.setRgb(255, 0, 0);//PCA color
        break;
    default:
        this->color.setRgb(100,100,100);
        break;
    }
}

void ProjectionView::setLocationX(float x)
{
    this->locationX = x;
}

void ProjectionView::setLocationY(float y)
{
    this->locationY = y;
}

void ProjectionView::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

bool ProjectionView::contains(QPointF pos)
{
    float x = pos.x();
    float y = pos.y();
    bool case1 = this->locationX < x && x < this->locationX + this->width;
    bool case2 = this->locationY < y && y < this->locationY + this->height;
    if(case1 && case2)
        return true;
    else
        return false;
}

bool ProjectionView::similar(ProjectionView *pv)
{
    float poutlying = pv->getOutlying();
    float pskewed   = pv->getSkewed();
    float pclumpy   = pv->getClumpy();
    float psparse   = pv->getSparse();
    float pstriated = pv->getStriated();
    float pconvex   = pv->getConvex();
    float pskinny   = pv->getSkinny();
    float pstringy  = pv->getStringy();
    float pmonotonic= pv->getMonotonic();

    float dis = pow(this->outlying - poutlying, 2);
    dis += pow(this->Skewed - pskewed, 2);
    dis += pow(this->clumpy - pclumpy, 2);
    dis += pow(this->sparse - psparse, 2);
    dis += pow(this->striated - pstriated, 2);
    dis += pow(this->convex - pconvex, 2);
    dis += pow(this->skinny - pskinny, 2);
    dis += pow(this->stringy - pstringy, 2);
    dis += pow(this->monotonic - pmonotonic, 2);
    return sqrt(dis) < 0.3;
}

void ProjectionView::metricsCalculator()
{
    /****************************************
    generate two dimensional scatter plot data
    *****************************************/
    QList<float> x_axis;
    QList<float> y_axis;
    std::vector<float>* entry;
    float xvalue;
    float yvalue;
    for(int r = 0; r < datasource->getDataEntrys(); r++){
       xvalue = 0;
       yvalue = 0;
       entry = datasource->getRow(r);
       for(int d = 0; d < entry->size(); d++){
           xvalue += xp->at(d)*entry->at(d);
           yvalue += yp->at(d)*entry->at(d);
       }
       x_axis.push_back(xvalue);
       y_axis.push_back(yvalue);
    }

    /********************************************************
    calcuate the metrics value of scatter plot.
    *********************************************************/
    metricsvalue = this->metrics->CalculateMultipleEmbeddingMetrics(x_axis,y_axis);
    this->outlying = metricsvalue[0];
    this->Skewed = metricsvalue[1];
    this->clumpy = metricsvalue[2];
    this->sparse = metricsvalue[3];
    this->striated = metricsvalue[4];
    this->convex = metricsvalue[5];
    this->skinny = metricsvalue[6];
    this->stringy = metricsvalue[7];
    this->monotonic = metricsvalue[8];
}

