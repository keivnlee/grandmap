#ifndef RENDERAREA
#define RENDERAREA
#include <QWidget>
#include "fileloader.h"
#include "Map/grandmap.h"
#include "Map/parallelcoordinate.h"
#include "Map/infopanel.h"
class RenderArea : public QWidget{

    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);

    void SamplingMethodChange(int method);
    void FliteringMethodChange(int method, int k = 0);
    void SampeNumberChange(int num);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);

private:
    int width ,height;
    FileLoader * loader;
    GrandMap* map;
    ParallelCoordinate *pcoordinate;
    InfoPanel* info;
    QRect Circle;

};

#endif // RENDERAREA

