#include "point.h"

Point::Point(float x, float y, int index, Storage *storage)
{
    this->valueX = x;
    this->valueY = y;
    this->width = 10;
    this->height = 10;
    this->setColor(-1);
    this->index = index;
    this->storage = storage;

    this->HintActive = false;
}

Point::~Point()
{

}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(this->color);
    painter->setBrush(brush);
    painter->drawEllipse(this->locationX, this->locationY, this->width, this->height);

    if(HintActive){
        this->drawHintTextBox(painter);
    }
}

QRectF Point::boundingRect() const
{

}

bool Point::contain(QPoint pos)
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
        this->color.setRgb(255,255,255);
        break;
    }
}

void Point::setGradientColor(int R, int G, int B)
{
    this->color.setRgb(R, G, B);
}

void Point::setHintActive(bool value)
{
    this->HintActive = value;
}

void Point::drawHintTextBox(QPainter* painter)
{
    painter->drawRect(locationX, locationY, 50, 50);
    painter->drawText(locationX, locationY, QString::number(storage->getItem(index, 0), 'f', 3));
}

