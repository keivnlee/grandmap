#ifndef NDPAD_H
#define NDPAD_H
#include <QFrame>
#include <QPainter>
#include <Eigen/Core>
#include <QMouseEvent>
#include "scatterplot.h"
#include "scatterplotinfopanel.h"
class NDPad : public QGraphicsItem
{
public:
    NDPad(float x, float y, QStringList *labels, Storage* datasource);
    ~NDPad();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    //need to use to set up the initial projection
    void setPlot(ScatterPlot* plot);
    void setInfo(ScatterPlotInfoPanel *info);
    void setInitalProjection(std::vector<float> *xp, std::vector<float> *yp);
    //base on current vector weight, reset the pad
    void resetPad();
    QRectF boundingRect() const;

private:
    bool isInPad(QPointF p); //check if point p is on this interface
    float getWeightHelper(QPointF loc, QPointF p1, QPointF p2, QPointF p3);
    void colorGuiderInitial();
    void getColor(float value, int *red, int *green, int *blue);
    Eigen::VectorXf getBarycentricCoordinateVector(float x, float y);
    Eigen::VectorXf get_Y_Vector(float x, float y);
    Eigen::VectorXf getOrthogonal_X_Vector();

    Eigen::VectorXf get_X_Vector(float x, float y);
    Eigen::VectorXf getOrthogonal_Y_Vector();
    Eigen::VectorXf Orthogonal(Eigen::VectorXf PX, Eigen::VectorXf PY);
    //x_axis, y_axis will be the two orthogonal vector of initial projection view.
    Eigen::VectorXf x_axis, y_axis, X_AXIS, Y_AXIS;

    Storage* datasource;
    QStringList* labels;
    ScatterPlot *plot;
    ScatterPlotInfoPanel *info;
    float x, y, gap, radius;
    int dimension;

    //parameter for the polygon
    QPolygon polyX, polyY;
    QRectF circleX, circleY;
    bool flag;

    // for color guider
        /*Color guider is used to trace the value distribution of one quality metrics in nd-touchpad*/
    QList<QRect*> padPoints;
    QList<float>  padPointsValue;
};

#endif // NDPAD_H
