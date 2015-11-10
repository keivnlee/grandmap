#ifndef SCATTERPLOTINFOPANEL_H
#define SCATTERPLOTINFOPANEL_H
#include <QGraphicsItem>
#include <Eigen/Core>
#include "../Map/projectionview.h"
class ScatterPlotInfoPanel : QGraphicsItem
{
public:
    ScatterPlotInfoPanel(float x,  float y, float width, float height,
                         QStringList* Labels, std::vector<float>* x_axis, std::vector<float>* y_axis);
    ~ScatterPlotInfoPanel();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    void setProjectionView( Eigen::VectorXf XP, Eigen::VectorXf YP);
private:

    float x, y, width, height;
    QStringList* Labels;
    ProjectionView* currentProjection;
    std::vector<float>* x_axis;
    std::vector<float>* y_axis;
};

#endif // SCATTERPLOTINFOPANEL_H
