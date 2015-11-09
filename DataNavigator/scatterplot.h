#ifndef SCATTERPLOT_H
#define SCATTERPLOT_H
#include <Eigen/Core>
#include "point.h"
#include "../storage.h"
#include "../Map/projectionview.h"
#include <Library/kmeans++/KMeans.h>
#include <QGraphicsItem>
#include "scatterplotinfopanel.h"
class ScatterPlot : QGraphicsItem
{
public:
    /*********************************************************************************
     Scatter plot is used to show the projectionview user select from the projection
     pool. The following information will be show out.
        1. a scatter plot used to show the source data base on the projection provide
        by the projection view you select.

        2. a biplot show the contribution of each dimension in the final 2d scatter plot

        3. a rotation bar which is used to help you check the neighbour of the projection
     *
     **********************************************************************************/
    //datasource is the original high dimensional data source
    //x and y is the start position of the scatter plot on the panel
    ScatterPlot(Storage* datasource, float x, float y, float width, float height, ProjectionView* p);
    ~ScatterPlot();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;


    void setProjectionView(ProjectionView* pv);
    //change the projection vector of current projection view.
    void setProjection(Eigen::VectorXf xv, Eigen::VectorXf yv);

private:

    /**********************************************************************
    point generator is used to generate the point base on the project vector of
    selected projection view.
    **************************************************************************/
    void pointGenerator();


    /***************************************************************************
     point position normalization is used to properly normalization the position
     value of each point and make sure all of them are in the panel.
     ****************************************************************************/
    void pointPositionNormalization();

    void KMean();

    float locationX;
    float locationY;
    float width;
    float height;

    std::vector<Point*>* point_pool;
    ProjectionView *selview;
    Storage *datasource;
    std::vector<float> x_axis, y_axis;


};

#endif // SCATTERPLOT_H
