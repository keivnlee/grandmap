#include "parallelcoordinate.h"
#include <QPainter>
#include <QCursor>
#include "float.h"
ParallelCoordinate::ParallelCoordinate(float x, float y, float width, float height, std::vector<ProjectionView*>* projectionview_pool)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->projectionview_pool = projectionview_pool;

    this->columnCount = 9;
    this->rowCount = this->projectionview_pool->size();

    this->axisPadding = (width - 100)/(columnCount-1);
    this->axisTopPadding = 15;
    this->axisBottomPadding = 10;
    this->axisX = new float[columnCount];
    for(int c = 0; c < columnCount; c++){
        axisX[c] = x + c * axisPadding + 50;
    }

    this->PathInitial();

    labels.append("outlying");
    labels.append("skewed");
    labels.append("clumpy");
    labels.append("sparse");
    labels.append("striated");
    labels.append("convex");
    labels.append("skinny");
    labels.append("stringy");
    labels.append("monotonic");


    for(int c = 0; c < columnCount; c++){
        QRect* rect =  new QRect(axisX[c]-10, y+60, 20, 0);
        QRect crect(axisX[c]-10, y+axisTopPadding, 20, height-axisTopPadding - axisBottomPadding);
        filterRect.append(rect);
        contentRect.append(crect);
    }
}

ParallelCoordinate::~ParallelCoordinate()
{

}

void ParallelCoordinate::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    PathFilter();
    QBrush white(Qt::white);
    painter->setBrush(white);
    painter->drawRect(x, y, width, height);
    this->drawPath(painter);
    this->drawAxes(painter);
    this->drawFilterRect(painter);


}

QRectF ParallelCoordinate::boundingRect() const
{

}

void ParallelCoordinate::mousePressEvent(QMouseEvent *event)
{
    QRect rect(event->pos().x(), event->pos().y(), 1, 1);
    for(int i = 0 ; i < contentRect.size(); i++){
        if(contentRect.at(i).intersects(rect)){
            filterRect.at(i)->setY(event->pos().y());
        }
    }
}

void ParallelCoordinate::mouseMoveEvent(QMouseEvent *event)
{
    QRect rect(event->pos().x(), event->pos().y(), 1, 1);
    for(int i = 0 ; i < contentRect.size(); i++){
        if(contentRect.at(i).intersects(rect)){
            filterRect.at(i)->setBottom(event->pos().y());
        }
    }
}

void ParallelCoordinate::mouseReleaseEvent(QMouseEvent *event)
{
    QRect rect(event->pos().x(), event->pos().y(), 1, 1);
    for(int i = 0 ; i < contentRect.size(); i++){
        if(contentRect.at(i).intersects(rect)){
            filterRect.at(i)->setBottom(event->pos().y());
        }
    }
    // this->unsetCursor();
}

void ParallelCoordinate::setProjectionViews(std::vector<ProjectionView *> *projectionview_pool)
{
    this->projectionview_pool = projectionview_pool;
    this->rowCount = this->projectionview_pool->size();
    this->PathInitial();
}

void ParallelCoordinate::drawAxes(QPainter* painter)
{

    float x1,x2,y1,y2,step,columnMin,columnMax,valueStep;
    QString label;
    QFont ten;
    QFont eight;
    ten.setPixelSize(10);
    eight.setPixelSize(8);
    QPen pen;
    QColor color(0, 0, 0, 255);
    pen.setColor(color);
    QBrush brush(Qt::black);
    painter->setBrush(brush);
    painter->setPen(pen);
    int TICK = 10;
    for(int c = 0; c < columnCount; c++){
        label = labels.at(c);
        x1 = axisX[c];
        x2 = x1;
        y1 = y + axisTopPadding;
        y2 = y + height - axisBottomPadding;
        painter->setFont(ten);
        painter->drawLine(x1, y1, x2, y2);
        painter->drawText( x1 - 25, y1 - 7, label);
        //draw Tick
        step = (y2 - y1)/TICK;
        columnMin = this->getColumnMin(c);
        columnMax = this->getColumnMax(c);
        valueStep = (columnMax - columnMin) / TICK;
        painter->setFont(eight);
        for(int i = 0; i <= TICK; i++){
            painter->drawLine(x1 -10, y1 + step * i, x1, y1+step*i);
            painter->drawText(x1 - 30, y1+step * i, QString::number(columnMax - valueStep * i, 'f', 2));
        }
    }


}

void ParallelCoordinate::drawPath(QPainter* painter)
{
   QColor unActive(221,221,221,0);
   QColor active(4, 100, 179, 100);
   QBrush brush;
   brush.setColor(unActive);
   painter->setBrush(brush);
   QPen pen;
   unActive.setAlpha(30);


   for(int i = 0; i < pathset.size(); i++){
       if(pathActive.at(i)){
           pen.setColor(active);
           painter->setPen(pen);
       }else{
           pen.setColor(unActive);
           painter->setPen(pen);
       }
       painter->drawPath(pathset.at(i));
   }

}

void ParallelCoordinate::drawFilterRect(QPainter *painter)
{
    QPen pen;
    QColor color(0, 0, 0);
    QBrush brush(Qt::blue);
    pen.setColor(color);
    painter->setPen(pen);
    color.setRgb(100, 100, 100, 100);
    brush.setColor(color);
    painter->setBrush(brush);

    QList<QRect*>::Iterator i;
    int height ,width,  x, y;
    for(i = filterRect.begin(); i != filterRect.end(); ++i){
        (*i)->getRect(&x, &y, &width, &height);
        if(abs(height) > 1)
            painter->drawRect(**i);
    }
}

void ParallelCoordinate::PathInitial()
{

    float y1, y2;
    y1 = this->y + this->axisTopPadding;
    y2 = this->y + height - this->axisBottomPadding;
    //path inital

    pathset.clear();
    pathActive.clear();

    float* columnMax = new float[columnCount];
    float* columnMin = new float[columnCount];
    for(int c = 0; c < columnCount; c++){
         columnMax[c] = this->getColumnMax(c);
         columnMin[c] = this->getColumnMin(c);
    }


    for(int r = 0; r < rowCount; r++){
        QPainterPath path;
        QPointF startPoint(axisX[0],y2 - (getColumnValue(projectionview_pool->at(r), 0) - columnMin[0])/(columnMax[0] - columnMin[0])*(y2 - y1));
         path.moveTo(startPoint);
        for(int c = 1; c < columnCount; c++){
            QPointF p2(axisX[c], y2 - (getColumnValue(projectionview_pool->at(r), c) - columnMin[c])/(columnMax[c] - columnMin[c])*(y2 - y1));
            path.lineTo(p2);
        }
        pathset.append(path);
        pathActive.append(false);
    }

}

void ParallelCoordinate::PathFilter()
{
    int r, c;
    QRect rect;
    bool flag;
    for(r = 0; r < rowCount; r++){
        flag = true;
        for(c = 0; c < columnCount; c++){
            rect.setRect(pathset.at(r).elementAt(c).x, pathset.at(r).elementAt(c).y, 1, 1);
            if(abs(filterRect.at(c)->height()) <= 1) continue;
            if(!filterRect.at(c)->intersects(rect)){
               this->pathActive.replace(r, false);
               this->projectionview_pool->at(r)->setVisible(false);
               flag = false;
               break;
            }
        }
        if(flag){
            this->pathActive.replace(r, true);
            this->projectionview_pool->at(r)->setVisible(true);
        }
    }
}

float ParallelCoordinate::getColumnMin(int column)
{
    float min = FLT_MAX;//all value will be smaller than 1;
    float value;
    for(int i = 0; i < projectionview_pool->size(); i++){
        value = this->getColumnValue(projectionview_pool->at(i), column);
        if(min > value)
                min = value;
    }
    return min;
}

float ParallelCoordinate::getColumnMax(int column)
{
    float max = FLT_MIN;//all value will be smaller than 1;
    float value;
    for(int i = 0; i < projectionview_pool->size(); i++){
        value = this->getColumnValue(projectionview_pool->at(i), column);
        if(max < value)
                max = value;
    }
    return max;
}

float ParallelCoordinate::getColumnValue(ProjectionView *pv, int column)
{
    switch(column){
        case 0:
            return pv->getOutlying();
        case 1:
            return pv->getSkewed();
        case 2:
            return pv->getClumpy();
        case 3:
            return pv->getSparse();
        case 4:
            return pv->getStriated();
        case 5:
            return pv->getConvex();
        case 6:
            return pv->getSkinny();
        case 7:
            return pv->getStringy();
        case 8:
            return pv->getMonotonic();
    }
}

