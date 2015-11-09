#include "point.h"

Point::Point(float x, float y)
{
    this->valueX = x;
    this->valueY = y;
    this->width = 10;
    this->height = 10;
    this->setColor(-1);
}

Point::~Point()
{

}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(this->color);
    painter->setBrush(brush);
    painter->drawEllipse(this->locationX, this->locationY, this->width, this->height);
}

QRectF Point::boundingRect() const
{

}

float Point::getX()
{
    return this->valueX;
}

float Point::getY()
{
    return this->valueY;
}

void Point::setValue(float x, float y)
{
    this->valueX = x;
    this->valueY = y;
}

void Point::setLocation(float x, float y)
{
    this->locationX = x;
    this->locationY = y;
}

void Point::setColor(int color)
{
    switch(color){
    case 0:
        this->color.setRgb(153, 102, 51);
        break;
    case 1:
        this->color.setRgb(255, 153, 0);
        break;
    case 2:
        this->color.setRgb(102, 102, 155);
        break;
    case 3:
        this->color.setRgb(255, 0, 0);
        break;
    case 4:
        this->color.setRgb(0, 102, 255);
        break;
    case 5:
        this->color.setRgb(0, 204, 0);
        break;
    case 6:
        this->color.setRgb(204, 153, 0);
        break;
    case 7:
        this->color.setRgb(204, 51, 0);
        break;
    case 8:
        this->color.setRgb(0, 102, 102);
        break;
    case 9:
        this->color.setRgb(102, 0, 204);
        break;
    default:
        this->color.setRgb(0,0,0);
        break;
    }
}

