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
    QBrush magenta(Qt::magenta);
    QBrush cyan(Qt::cyan);
    painter->setBrush(brush);
    painter->drawRect(x, y, width + 60, height);

    for(int i = 0; i < Labels->size(); i++){
        painter->drawText(x + 50 + i * width/Labels->size(), y + 10, Labels->at(i));
        if(x_axis->at(i) > 0)
            painter->setBrush(cyan);
        else
            painter->setBrush(magenta);
        painter->drawRect(x + 60 + i * width/Labels->size(), y + 60,  20, -30 * x_axis->at(i));

        if(y_axis->at(i) > 0)
            painter->setBrush(cyan);
        else
            painter->setBrush(magenta);
        painter->drawRect(x + 60 + i * width/Labels->size(), y + 120, 20, -30 * y_axis->at(i));
    }

    painter->drawLine(x + 40, y + 60,  x + width+40, y + 60);
    painter->drawLine(x + 40, y + 120, x + width+40, y + 120);
    painter->drawText(x + 5,  y + 63, "X-Axis");
    painter->drawText(x + 5,  y + 123, "Y-Axis");
}

QRectF ScatterPlotInfoPanel::boundingRect() const
{

}

void ScatterPlotInfoPanel::setProjectionView(Eigen::VectorXf XP, Eigen::VectorXf YP)
{
    this->x_axis->clear();
    this->y_axis->clear();
    for(int r = 0; r< XP.rows(); r++){
        this->x_axis->push_back(XP[r]);
        this->y_axis->push_back(YP[r]);
    }
}

