#include "infopanel.h"
#include <QPainter>
#include <QDebug>
#include <float.h>
InfoPanel::InfoPanel(float x, float y, float width, float height ,
                     std::vector<float> *X_axis, std::vector<float> *Y_axis, std::vector<float>*distance)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->X_axis = X_axis;
    this->Y_axis = Y_axis;
    this->distance = distance;

    labels.append("outlying");
    labels.append("skewed");
    labels.append("clumpy");
    labels.append("sparse");
    labels.append("striated");
    labels.append("convex");
    labels.append("skinny");
    labels.append("stringy");
    labels.append("monotonic");


    //bar char parameter inital
    barColumn = 11;
    count = new int[barColumn];
    for(int i = 0; i < barColumn; i++)
        count[i] = 0;
    valueMax = FLT_MIN;
    valueMin = FLT_MAX;
    for(int i = 0; i < distance->size(); i++){
        if(valueMin > distance->at(i)){
            valueMin = distance->at(i);
        }
        if(valueMax < distance->at(i)){
            valueMax = distance->at(i);
        }
    }
    for(int i = 0; i < distance->size(); i++){
        count[BarChar_ColumnIndex(distance->at(i))]++;
    }

    barX = x;
    barY = y + 300;
    barWidth = width/1.5;
    barHeight= 200;

}

InfoPanel::~InfoPanel()
{

}

void InfoPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(Qt::white);
    painter->setBrush(brush);
    //painter->drawRect(x, y + 200, width/2, height/2);

    for(int i = 0 ; i < labels.size(); i++){
        painter->drawText(x+10+i*width/labels.size(), y-20, labels[i]);
    }

    for(int i = 0 ; i < X_axis->size(); i++){
        painter->drawRect(x+10+i*width/X_axis->size(), y + 50, 20, (X_axis->at(i)*30));
        painter->drawRect(x+10+i*width/Y_axis->size(), y + 120, 20, (Y_axis->at(i)*30));
    }
    painter->drawLine(x, y + 50, x + width, y + 50);
    painter->drawLine(x, y + 120, x + width, y + 120);
    painter->drawText(x - 50, y + 50, "X-Axis");
    painter->drawText(x - 50, y + 120, "Y-Axis");

    //this->drawProjectionDistanceDistributionBarChart(painter);
}

QRectF InfoPanel::boundingRect() const
{

}

int InfoPanel::BarChar_ColumnIndex(float value)
{
    float gap = (valueMax - valueMin)/barColumn;
    for(int  i = 0; i < barColumn-1; i++){
        if(value >= (valueMin + gap * i)  && value < (valueMin + gap * (i+ 1)))
            return i;
    }
    return barColumn-1;
}

void InfoPanel::drawProjectionDistanceDistributionBarChart(QPainter *painter)
{
    for(int i = 0; i < barColumn; i++){
        painter->drawRect(barX + barWidth/barColumn * i, barY, barWidth/barColumn, -barHeight * count[i]/distance->size());
        painter->drawText(barX + barWidth/barColumn * i - 13, barY + 15, QString::number(valueMin + (valueMax - valueMin)/barColumn * i, 'f', 3));
    }
    painter->drawText(barX+ barWidth - 13, barY + 15, QString::number(valueMax, 'f', 3));
}


