#ifndef PROJECTIONVIEW_H
#define PROJECTIONVIEW_H
#include <QGraphicsItem>
#include <QPainter>
#include "../storage.h"
#include "../Scagnostics/GraphTheoreticScagnosticsMetric.h"
#include "../ALGLIB/src/ap.h"
#include "../ALGLIB/src/dataanalysis.h"
class ProjectionView : public QGraphicsItem
{
    /*******************************************************************************
         projectionview is one of the andrew curve projection.
         it contains two dimension data which is a high dimensional data projection into
         2-dimensional space. It will be represented as a point in the scatterplot tour

         The following information will be contain in the projectionview
         1. for each scatterplot, we setup 9 metrics, there are outlying, skewed, clumpy,
            sparse, striatd, convex, skinny, stringy, and monotonic.

         2. the xp, and yp are two orthogonal vectors which are used to project the high
            dimensional data into x-axis and y-axis.

     *******************************************************************************/
public:
    ProjectionView(std::vector<float>* xp, std::vector<float>* yp, Storage* storage);
    ~ProjectionView();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;


    std::vector<float> *getXP();
    std::vector<float> *getYP();
    float getMetricsValue(int index);
    float getOutlying();
    float getSkewed();
    float getClumpy();
    float getSparse();
    float getStriated();
    float getConvex();
    float getSkinny();
    float getStringy();
    float getMonotonic();
    float getX();
    float getY();
    QColor getColor();
    float getLocationX();
    float getLocationY();
    bool getVisible();

    void setVisible(bool v);
    void setX(float x);
    void setY(float y);
    void setColor(int color);
    void setLocationX(float x);
    void setLocationY(float y);
    void setSize(int width, int height);

    /***************************************************************
         Projection view will be shown as a point in the scatter plot.
         the point is defined by four variables

                    location_X
                    location_Y
                    width
                    heigth
    ****************************************************************/
    bool contains(QPointF pos);
    /***************************************************************
         * similar calcuate the distance between pairs of projection view
         * and we setup a threadhold which help us to indicate those two
         * projection view are similar to each other or not. The similarity
         * are based on the Euclidean distance with those 9 attributes
         * sqrt[ (p1.outlyting - p2.outlying)^2 + .... ]
    *******************************************************************/
    bool similar(ProjectionView* pv);

private:

   /************************************************************************
    Base on the xp, yp which are the x-axis, and y-axis projector. we can generate
    two dimensional data set.With those two dimensional data set, we can use the scatter
    plot matrix to generate outlying, Skewed, clumpy, striated,convex, skinny, stringy,
    and monotonic different metrics.
    *************************************************************************/
   void metricsCalculator();
   /*************************************************************************
    The color protocol:
       0 represent black color identify normal projectionview
       1 represent gray  color identify less interesting projectionivew
       2 represent red   color identify PCA projectionview
       3 represent blue  color identify
    **************************************************************************/

   std::vector<float>* xp;
   std::vector<float>* yp;
   std::vector<float> metricsvalue;
   //scagnostic metrics
   float outlying;
   float Skewed;
   float clumpy;
   float sparse;
   float striated;
   float convex;
   float skinny;
   float stringy;
   float monotonic;

   //value of the projectionview in the scatterplot
   float x;
   float y;
   QColor color;

   //location of the projectionview in the scatter plot
   float locationX;
   float locationY;


   //if the projection view is visible on the grand map or not
   bool visible;
   int width;
   int height;
   Storage* datasource;
   DataExplorerHD::GraphTheoreticScagnosticsMetric* metrics;

};

#endif // PROJECTIONVIEW_H
