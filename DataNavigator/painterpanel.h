#ifndef PAINTERPANEL_H
#define PAINTERPANEL_H

#include <QWidget>
#include "scatterplot.h"
#include "../storage.h"
#include "ndpad.h"

class PainterPanel : public QWidget
{
    Q_OBJECT
public:
    explicit PainterPanel(Storage* storage, ProjectionView *p, QWidget *parent = 0);
    ~PainterPanel();
    void setProjectionView(ProjectionView* p);
    void attributeChange(int index);
    void buttonClick();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    ScatterPlot *plot;
    Storage* storage;
    NDPad *pad;
    ScatterPlotInfoPanel* info;
    int width ,height;
};

#endif // PAINTERPANEL_H
