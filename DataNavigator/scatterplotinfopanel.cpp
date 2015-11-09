#include "scatterplotinfopanel.h"

ScatterPlotInfoPanel::ScatterPlotInfoPanel(float x,  float y, float width, float height,
                                           QStringList* Labels, std::vector<float>* x_axis, std::vector<float>* y_axis)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->Labels = Labels;
    this->x_axis = x_axis;
    this->y_axis = y_axis;
}

ScatterPlotInfoPanel::~ScatterPlotInfoPanel()
{

}

void ScatterPlotInfoPanel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush(Qt::white);
    painter->setBrush(brush);
    painter->drawRect(x, y, width, height);

    for(int i = 0; i < Labels->size(); i++){
        painter->drawText(x + 50 + i * width/Labels->size(), y + 10, Labels->at(i));
        painter->drawRect(x + 50 + i * width/Labels->size(), y + 50,  20, -50 * x_axis->at(i));
        painter->drawRect(x + 50 + i * width/Labels->size(), y + 110, 20, -50 * y_axis->at(i));
    }
}

QRectF ScatterPlotInfoPanel::boundingRect() const
{

}

void ScatterPlotInfoPanel::setProjectionView(std::vector<float>* x_axis, std::vector<float>* y_axis)
{
    this->x_axis = x_axis;
    this->y_axis = y_axis;
}

