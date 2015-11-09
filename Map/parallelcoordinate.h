#ifndef PARALLELCOORDINATE_H
#define PARALLELCOORDINATE_H
#include "projectionview.h"
#include <QGraphicsItem>
#include <QMouseEvent>
class ParallelCoordinate : QGraphicsItem
{
public:
    ParallelCoordinate(float x, float y, float width, float height, std::vector<ProjectionView*>* projectionview_pool);
    ~ParallelCoordinate();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


    void setProjectionViews(std::vector<ProjectionView*>* projectionview_pool);
private:


    //basic drawing function
    void drawAxes(QPainter *painter);
    void drawPath(QPainter *painter);
    void drawFilterRect(QPainter *painter);
    void PathInitial();
    void PathFilter();

    float getColumnMin(int column);
    float getColumnMax(int column);
    float getColumnValue(ProjectionView* pv, int column);
    //axie label
    QStringList labels;

    //the coordinate position of x,y. and the canvas width and height of the plot
    float x, y, width, height;
    //the number of the axis of parallel and the size of data show on it.
    int columnCount, rowCount;
    //axis padding
    float axisPadding, axisTopPadding, axisBottomPadding;
    //x-coordination of each axe
    float* axisX;

    //filter rect
    QList<QRect*> filterRect;

    QList<QRect> contentRect;
    QList<QPainterPath> pathset;
    QList<bool> pathActive;

    //projection view pool
    std::vector<ProjectionView*>* projectionview_pool;
};

#endif // PARALLELCOORDINATE_H
