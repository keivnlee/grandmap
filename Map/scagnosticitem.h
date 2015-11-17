#ifndef SCAGNOSTICITEM_H
#define SCAGNOSTICITEM_H
#include <QRect>
#include <QPainter>
#include <QGraphicsItem>
#include "projectionview.h"
class ScagnosticItem : QGraphicsItem
{
public:
    ScagnosticItem(int row, int column, float x, float y, float width, float height,
                   std::vector<ProjectionView *> *projectionview_pool);
    ~ScagnosticItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    bool contain(QPoint pos);
    int getRow();
    int getColumn();
    void setActive(bool flag);

private:


    std::vector<float> xAxis_data, yAxis_data;
    float x, y, width, height;
    int row, column;//indicate row and coloumn scatter plot in the Scagnostic.
    bool flag;//if the plot is current active
    QRect Canvas;
};

#endif // SCAGNOSTICITEM_H
