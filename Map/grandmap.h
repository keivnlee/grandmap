#ifndef GRANDMAP_H
#define GRANDMAP_H
#include <QGraphicsItem>
#include<QPainter>
#include<QMouseEvent>
#include "storage.h"
#include "projectionview.h"
#include <Eigen/Core>
#include <Library/kmeans++/KMeans.h>
#include <DataNavigator/projectionpanel.h>
class GrandMap : public QGraphicsItem
{
        /*********************************************************************
            PMap use the Wegman's/Grand Tour/Projection Pursuit tour algorithm to produces amounts
          of different 2 orthogonal vectors.
            For each pair orthogonal vectors we can generate a two dimensional scatter plot and for each of them,
          we take it as an image and we use the 9 metrics
                1.outlying
                2.monotonic
                3.skewed
                4.convex
                5.striated
                6.straight
                7.skinny
                8.stringy
                9.clumpy
         to measure each scatter plot.
         **********************************************************************/
#define CHORDAL_DIST_MULTIPLIER 0.70711


public:
    GrandMap(Storage * datasource, float x, float y, int width, int height);
    ~GrandMap();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    /********************************************************************************
        base on the projection from the andrew curve, it will generate 360 different
        scatter plot.
    *********************************************************************************/
    void projectionviewGenerator(QString method);

    /********************************************************************************
     so far the mouse event only try to handle the mouse click event. When the mouse
     click one projectionview, the program will identify which projection view is
     selected and show the projection view in the scatter plot
    *********************************************************************************/
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool isInPad(QPointF p);

    std::vector<ProjectionView *> *getProjectionViews();
    std::vector<float> *getXP();
    std::vector<float> *getYP();
    std::vector<float> *getProjectionDistance();
    void setProjection(Eigen::VectorXf xp, Eigen::VectorXf yp, int row, int column);
    float getProjecitonViewMaximumDistance();
    void setSimilarDistance(float distance);
    //once the sample in the projection pool change, renormalize
    //projection view's position
    void Update();

    //change the total sample size in the projection view pool
    void setSampleSize(int num);

    //filering projection view base on the pass in method
    void Filtering(QString method, int k = 0);
private:
    void PCA();
    /********************************************************************
       projection helper projection the 9 feature of scatter plot into the
       direction of pv vector
    *********************************************************************/
    float projectionhelper(ProjectionView *p, std::vector<float>* pv);

    void init_Max_Min();

    /*********************************************************************
     * Target Projection pursuit
     *
     *          min || T - XP ||
     *
     * T is the target matrix, P is the candidate projection matrix and X is the
     * datasource matrix. In Procruste analysis, we use singular value decomposition
     * to find out the projection vector which result into the optimize result in
     * the above formula.
     *
     *              mx represent the distance that selected point move in x-axis
     *              my represent the distance that selected point move in y-axis
     *
     * We add the distance to the selected point and the new point set will be the
     * target matrix we can looking for.
     *
     * Algorithm description:
     *              M = A^T B     A is the data source matrix and B is the target matrix
     *              # R is the projection matrix we are looking for
     *              M = U E V     singular value decomposition
     *              R = U * V
     *
     **********************************************************************/
    void targetProjectionPursuit(float mx, float my);

    /********************************************************************
     Normalization is used to normalize the value of each projectionview
     and make sure that all the view will be shown in the scatter plot tour.
     normalization formula:
              new_value = (old_value - min)/(max - min)
     ********************************************************************/
     void normalizaton();

     /***************************************************************
        GT_At_Random use the math model proposed by in the grand tour paper.
        Each time, the model will generate one randome projection in high dimensional
        space.
      *************************************************************/
     std::vector<std::vector<float> *> *GT_At_Random();

     /****************************************************
        Generate one randon 2d-projection base on current data dimension
            d: the dimension of the data set
      *****************************************************/
     Eigen::VectorXf randomVector(int d);

    /********************************************************************************
          Wegman's algorithm provide a two orthogonal projeciton vector.
          w1 = sqrt(2/d)(sin(x1*t), cos(x1*t), sin(x2*t), cos(x2*t),...,)
          w2 = sqrt(2/d)(cos(x1*t), sin(x1*t), cos(x2*t), sin(x2*t),...,)
     *********************************************************************************/
     std::vector<std::vector<float> *> *WegmanAlgorithm(float degree);

    /********************************************************************************
         Generate a PCA projection base on the orignal data source.
     *********************************************************************************/
     ProjectionView *get_PCA_Projectionview();


     /************************************************************************
      * K-means, generate k representive projection view
      *********************************************************************/
     void KMean(int k);

     //grassmannian distance
     float ChordalDistance(Eigen::MatrixXf & baseA, Eigen::MatrixXf & basesB);
     void ProjectionDistanceDistributionCalculation();

     float getProjectionEuclideanDistance(ProjectionView* p1, ProjectionView* p2);
     void  generateRepresentitiveProjectionView();
     float back_rescaleX(float x);
     float back_rescaleY(float y);

     ProjectionPanel* AnalysisPanel;
     Storage* datasource;
     std::vector<ProjectionView*>* projectionview_pool;

     //candidate_projectionview is a vector hold the representitive projection view
     std::vector<ProjectionView*>* candidate_projectionview;
     std::vector<float>* xp;
     std::vector<float>* yp;
     std::vector<float>* distance;
     QStringList labels;
     float max_x;
     float min_x;
     float max_y;
     float min_y;

     float x;
     float y;

     int width;
     int height;

     int size;//sample size of the projection view
     //float distance; // the similiarity distance between two projeciton view

     int row, column;

     //draw axis
     QLine lineX, Xarrow1, Xarrow2, lineY, Yarrow1, Yarrow2;
     QFont serifFont;
};

#endif // GRANDMAP_H
