#include "scagnosticitem.h"
ScagnosticItem::ScagnosticItem(int row, int column, float x, float y, float width, float height, std::vector<ProjectionView*>* projectionview_pool)
{
    this->row = row;
    this->column = column;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    Canvas.setRect(x, y, width, height);

    for(int i = 0 ; i < projectionview_pool->size(); i++){
        xAxis_data.push_back(projectionview_pool->at(i)->getMetricsValue(column));
        yAxis_data.push_back(projectionview_pool->at(i)->getMetricsValue(row));
    }

    flag = false;
}

ScagnosticItem::~ScagnosticItem()
{

}

void ScagnosticItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{   QBrush white(Qt::white);
    QBrush green(Qt::green);
    if(flag)
        painter->setBrush(green);
    else
        painter->setBrush(white);
    painter->drawRect(this->Canvas);
    for(int i = 0; i < xAxis_data.size(); i++){
        painter->drawEllipse(x + xAxis_data[i]*(width-4)+2, y + height - yAxis_data[i] * (height - 4)-2, 2, 2);
    }
}

QRectF ScagnosticItem::boundingRect() const
{

}

bool ScagnosticItem::contain(QPoint pos)
{
    QRect rect(pos.x(), pos.y(), 1, 1);
    return Canvas.intersects(rect);
}

int ScagnosticItem::getRow()
{
    return row;
}

int ScagnosticItem::getColumn()
{
    return column;
}

void ScagnosticItem::setActive(bool flag)
{
    this->flag = flag;
}

