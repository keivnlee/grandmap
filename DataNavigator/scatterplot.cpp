#include "scatterplot.h"

ScatterPlot::ScatterPlot(Storage *datasource, float x, float y, float width, float height, ProjectionView* p)
{
    this->datasource = datasource;
    this->point_pool = new std::vector<Point*>();
    this->locationX = x;
    this->locationY = y;
    this->width = width;
    this->height = height;
    this->setProjectionView(p);

}

ScatterPlot::~ScatterPlot()
{

}

void ScatterPlot::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush white(Qt::white);
    painter->setBrush(white);
    painter->drawRect(this->locationX-10, this->locationY-10,this->width+20, this->height+20);

    for(int i = 0; i< point_pool->size(); i++){
        point_pool->at(i)->paint(painter, option, widget);
    }
}

QRectF ScatterPlot::boundingRect() const
{

}

void ScatterPlot::setProjectionView(ProjectionView *pv)
{
    this->selview = pv;
    std::vector<float>* xp = this->selview->getXP();
    std::vector<float>* yp = this->selview->getYP();
    this->x_axis.clear();
    this->y_axis.clear();
    for(int i = 0; i < xp->size(); i++){
        this->x_axis.push_back(xp->at(i));
        this->y_axis.push_back(yp->at(i));
    }

    this->pointGenerator();
    this->pointPositionNormalization();
}

void ScatterPlot::setProjection(Eigen::VectorXf xv, Eigen::VectorXf yv)
{
    this->x_axis.clear();
    this->y_axis.clear();
    for(int r = 0; r < xv.rows(); r++){
        this->x_axis.push_back(xv[r]);
        this->y_axis.push_back(yv[r]);
    }

    this->pointGenerator();
    this->pointPositionNormalization();

}

void ScatterPlot::pointGenerator()
{
    int dimension = datasource->getDimension();
    int entrys    = datasource->getDataEntrys();
    float x = 0;
    float y = 0;

    if(this->point_pool->size() == 0){
        for(int e = 0; e < entrys; e++){
            x = 0;
            y = 0;
            for(int d = 0; d < dimension; d++){
                x += (this->x_axis.at(d) * this->datasource->getItem(e,d));
                y += (this->y_axis.at(d) * this->datasource->getItem(e,d));
            }
            this->point_pool->push_back(new Point(x, y));
        }
        this->KMean();
    }else{
        for(int e = 0; e < entrys; e++){
            x = 0;
            y = 0;
            for(int d = 0; d < dimension; d++){
                x += (this->x_axis.at(d) * this->datasource->getItem(e,d));
                y += (this->y_axis.at(d) * this->datasource->getItem(e,d));
            }
            this->point_pool->at(e)->setValue(x, y);
        }
    }
}

void ScatterPlot::pointPositionNormalization()
{
    float max_x = this->point_pool->at(0)->getX();
    float min_x = max_x;
    float max_y = this->point_pool->at(0)->getY();
    float min_y = max_y;
    Point* p;
    float locationX = 0;
    float locationY = 0;
    for(int i = 1; i < this->point_pool->size(); i++){
        p = this->point_pool->at(i);
        if(max_x < p->getX())
            max_x = p->getX();

        if(min_x > p->getX())
            min_x = p->getX();

        if(max_y < p->getY())
            max_y = p->getY();

        if(min_y > p->getY())
            min_y = p->getY();
    }

    for(int i = 0; i < this->point_pool->size(); i++){
        p = this->point_pool->at(i);
        locationX = this->locationX + (p->getX() - min_x)/(max_x - min_x) * this->width;
        locationY = this->locationY + (p->getY() - min_y)/(max_y - min_y) * this->height;
        p->setLocation(locationX, locationY);
    }
}

void ScatterPlot::KMean()
{
    int n = point_pool->size();
    int d = datasource->getDimension();
    int k = 3;
    Scalar points[n*d];
    for(int i = 0; i < n; i++){
        points[i*d+0] = datasource->getItem(i,0);
        points[i*d+1] = datasource->getItem(i,1);
        points[i*d+2] = datasource->getItem(i,2);
        points[i*d+3] = datasource->getItem(i,3);
    }
    Scalar centers[k*d];
    int assignments[n];
    RunKMeansPlusPlus(n, k, d, points, 5, centers, assignments);

    for(int i = 0; i < n; i++){
        this->point_pool->at(i)->setColor(assignments[i]);
    }
}

