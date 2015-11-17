#include "scagnostic.h"

Scagnostic::Scagnostic(float x, float y, float width, float height,
                       std::vector<ProjectionView *> *projectionview_pool)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    for(int r = 0; r < 9; r++){
        for(int c = 0; c <= r; c++){
            items.push_back(new ScagnosticItem(r, c, x + c/9.0 * width, y + r/9.0 * height, width/9, height/9,
                                               projectionview_pool));
        }
    }
    labels.append("outlying");
    labels.append("skewed");
    labels.append("clumpy");
    labels.append("sparse");
    labels.append("striated");
    labels.append("convex");
    labels.append("skinny");
    labels.append("stringy");
    labels.append("monotonic");
}

Scagnostic::~Scagnostic()
{

}

void Scagnostic::mousePressEvent(QMouseEvent *event)
{
    bool flag = true;
    for(int i = 0; i < items.size(); i++){
        if(items.at(i)->contain(event->pos()))
            flag = false;
    }

    if(flag) return;

    Eigen::VectorXf xp, yp;
    ScagnosticItem* item;
    xp.resize(9,1);
    yp.resize(9,1);
    xp.setZero(9);
    yp.setZero(9);
    for(int i = 0; i < items.size(); i++){
        item = items.at(i);
        if(item->contain(event->pos())){
            xp[item->getColumn()] = 1;
            yp[item->getRow()] = 1;
            map->setProjection(xp, yp, item->getRow(), item->getColumn());
            item->setActive(true);
        }else{
            item->setActive(false);
        }
    }
}

void Scagnostic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(Qt::white);
    painter->setBrush(brush);
    painter->drawRect(x, y, width, height);
    for(int i = 0; i < items.size(); i++){
        items.at(i)->paint(painter, option, widget);
    }
    //column labels
    for(int i = 0; i < labels.size(); i++){
        painter->drawText(x + width/9 * i + width/9/4, y - 10, labels.at(i));
    }

    //row labels
    for(int i = 0; i < labels.size(); i++){
        painter->drawText(x - 50, y + height/9 * i+ height/18, labels.at(i));
    }
}

QRectF Scagnostic::boundingRect() const
{

}

void Scagnostic::setMap(GrandMap *map)
{
    this->map = map;
}

