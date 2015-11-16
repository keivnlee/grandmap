#ifndef POINT_H
#define POINT_H
#include <QGraphicsItem>
#include <QPainter>
#include "../storage.h"
class Point : QGraphicsItem
{
public:
    Point(float x, float y, int index, Storage* storage);
    ~Point();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    bool contain(QPoint pos);

    float getX();
    float getY();

    //set the value of the point
    void setValue(float x, float y);

    //define the location of the point in the panel.
    void setLocation(float x, float y);

    //define the cluster color of the point
    void setColor(int color);
    void setGradientColor(int R, int G, int B);
    void setHintActive(bool value);
    void drawHintTextBox(QPainter *painter);

private:
    float width;
    float height;

    float valueX;
    float valueY;

    float locationX;
    float locationY;
    QColor color;

    int index;//the index of data entry in the data set.
    bool HintActive;
    Storage* storage;
};

#endif // POINT_H
