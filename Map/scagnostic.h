#ifndef SCAGNOSTIC_H
#define SCAGNOSTIC_H
#include <QRect>
#include <QPainter>
#include <QGraphicsItem>
#include <QMouseEvent>
#include "scagnosticitem.h"
#include "grandmap.h"
class Scagnostic : public QGraphicsItem
{
public:
    Scagnostic(float x, float y, float width, float height,
               std::vector<ProjectionView *> *projectionview_pool);
    ~Scagnostic();
    void mousePressEvent(QMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    void setMap(GrandMap* map);
private:
    GrandMap* map;

    float x, y, width, height;
    QRect Canvas;
    QList<QString> labels;
    std::vector<ScagnosticItem*> items;
};

#endif // SCAGNOSTIC_H
