#include "grandmap.h"
#include <QDebug>
GrandMap::GrandMap(Storage *datasource, float x, float y, int width, int height)
{
    this->datasource = datasource;
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
    this->size = 50;
    this->projectionviewGenerator("WE");
    candidate_projectionview = new std::vector<ProjectionView*>();
    xp = new std::vector<float>();
    yp = new std::vector<float>();
    distance = new std::vector<float>();
    this->AnalysisPanel = NULL;
    //in PCA, assign xp, and yp two different vector;
    this->PCA();
    this->init_Max_Min();
    this->normalizaton();
    ProjectionDistanceDistributionCalculation();


    row = -1;
    column = -1;
    labels.append("outlying");
    labels.append("skewed");
    labels.append("clumpy");
    labels.append("sparse");
    labels.append("striated");
    labels.append("convex");
    labels.append("skinny");
    labels.append("stringy");
    labels.append("monotonic");

    //axis component initial
        //x-axis
    lineX.setLine(x-20, y + height + 10, x + width + 50, y + height + 10);
    Xarrow1.setLine(x + width + 50, y + height + 10, x + width + 35, y + height+5);
    Xarrow2.setLine(x + width + 50, y + height + 10, x + width + 35, y + height+15);
        //y-axis
    lineY.setLine(x-20, y + height + 10, x - 20, y - 30);
    Yarrow1.setLine(x - 20, y - 30, x - 25, y - 15);
    Yarrow2.setLine(x - 20, y - 30, x - 15, y - 15);
    serifFont.setFamily("Times");
    serifFont.setPixelSize(10);
    serifFont.setBold(true);

    outputMetricsValue();
}

GrandMap::~GrandMap()
{

}

void GrandMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush white(Qt::white);
    painter->setBrush(white);
    painter->drawRect(x-20, y-10,this->width+40, this->height+20);
    //draw axis label
    painter->drawLine(lineX);
    painter->drawLine(lineY);
    painter->drawLine(Xarrow1);
    painter->drawLine(Xarrow2);
    painter->drawLine(Yarrow1);
    painter->drawLine(Yarrow2);
    painter->setFont(serifFont);
    if(row ==-1 && column == -1){
        painter->drawText(x + width + 55, y + height + 10, "X-Axis");
        painter->drawText(x - 10, y - 20, "Y-Axis");

    }else{
        painter->drawText(x + width + 55, y + height + 10, labels.at(column));
        painter->drawText(x - 10, y - 20, labels.at(row));
        painter->drawText(x - 20, y+height+5, "0");
        painter->drawText(x+width, y+height+5, "1");
        painter->drawText(x - 20, y, "1");
    }

    for(int i = 0; i < this->projectionview_pool->size(); i++){
        projectionview_pool->at(i)->paint(painter, option, widget);
    }

}

QRectF GrandMap::boundingRect() const
{

}

void GrandMap::projectionviewGenerator(QString method)
{
    std::vector<std::vector<float> *>* vp;
    std::vector<float>* vx;
    std::vector<float>* vy;

    if(!this->projectionview_pool){
        for(int i = 0; i < this->projectionview_pool->size(); i++)
            delete projectionview_pool->at(i);
        delete this->projectionview_pool;
    }
    std::vector<ProjectionView *>* projectionview_pool = new std::vector<ProjectionView *>();
    if(method == "GT")
        for(int i = 0; i < this->size; i++){
            vp = this->GT_At_Random();
            vx = vp->at(0);
            vy = vp->at(1);
            projectionview_pool->push_back(new ProjectionView(vx, vy, this->datasource));
            qDebug() << i;
        }
    else if(method == "WE")
        for(int degree = 0; degree < this->size; degree++){
            vp = this->WegmanAlgorithm(degree);
            vx = vp->at(0);
            vy = vp->at(1);
            projectionview_pool->push_back(new ProjectionView(vx, vy, this->datasource));
            qDebug() << degree;
        }
    else
        exit(0);

    projectionview_pool->push_back(this->get_PCA_Projectionview());
    this->projectionview_pool = projectionview_pool;
}

void GrandMap::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = event->pos();
    ProjectionView * pview;
    //if(!this->isInPad(pos))
        event->ignore();
    for(int i = 0; i < projectionview_pool->size(); i++){
          pview = projectionview_pool->at(i);
          if(pview->getVisible() && pview->contains(pos)){
             if(!this->AnalysisPanel){
                 this->AnalysisPanel = new ProjectionPanel(this->datasource, pview);
                 this->AnalysisPanel->show();
             }else{
                 this->AnalysisPanel->show();
                 this->AnalysisPanel->setProjectionView(pview);
             }
             pview->setSize(20,20);
          }else{
              pview->setSize(10,10);
          }
    }
}

void GrandMap::mouseMoveEvent(QMouseEvent *event)
{

}

void GrandMap::mouseReleaseEvent(QMouseEvent *event)
{

}

bool GrandMap::isInPad(QPointF p)
{
    float rx = p.rx();
    float ry = p.ry();
    // some point show up in the edge of the scatter plot
    // the 5 here make sure that those point can be marked
    bool case1 = (x - 10 < rx) && (rx < x + this->width + 10);
    bool case2 = (y - 10 < ry) && (ry < y + this->height+ 10);

    if(case1 && case2)
        return true;
    else
        return false;
}

std::vector<ProjectionView *> *GrandMap::getProjectionViews()
{
    return this->projectionview_pool;
}

std::vector<float> *GrandMap::getXP()
{
    return this->xp;
}

std::vector<float> *GrandMap::getYP()
{
    return this->yp;
}

std::vector<float> *GrandMap::getProjectionDistance()
{
    return this->distance;
}

void GrandMap::setProjection(Eigen::VectorXf xp, Eigen::VectorXf yp, int row, int column)
{
    this->xp->clear();
    this->yp->clear();
    for(int r = 0; r < xp.rows(); r++){
        this->xp->push_back(xp[r]);
        this->yp->push_back(yp[r]);
    }
    this->init_Max_Min();
    //this->normalizaton();
    ProjectionView* pview;
    for(int p = 0; p < projectionview_pool->size(); p++){
        pview = projectionview_pool->at(p);
        pview->setLocationX(pview->getX()*this->width + this->x);
        pview->setLocationY(-pview->getY()*height +height+ this->y);
    }

    this->row = row;
    this->column = column;
}

float GrandMap::getProjecitonViewMaximumDistance()
{
    int size = projectionview_pool->size();
    ProjectionView* p1 = projectionview_pool->at(0);
    ProjectionView* p2 = projectionview_pool->at(1);
    float maximum = this->getProjectionEuclideanDistance(p1, p2);
    float dis = 0;

    for(int i = 0; i < size; i++){
        p1 = projectionview_pool->at(i);
        for(int j = i+1; j < size; j++){
           p2 = projectionview_pool->at(j);
           dis = this->getProjectionEuclideanDistance(p1, p2);
           if(dis > maximum)
               maximum = dis;
        }
    }
    return maximum;

}

void GrandMap::setSimilarDistance(float distance)
{
    //this->distance = distance;
    this->generateRepresentitiveProjectionView();
}

void GrandMap::Update()
{
    this->init_Max_Min();
    this->normalizaton();
}

void GrandMap::setSampleSize(int size)
{
    this->size = size;
}

void GrandMap::Filtering(QString method, int k)
{
    if(method == "NC"){
        int i = 0;
        for(; i < projectionview_pool->size()-1; i++){
            projectionview_pool->at(i)->setColor(-1);
        }
        projectionview_pool->at(i)->setColor(9);//9 is PCA color
    }else if(method == "K-Mean"){
        this->KMean(k);
    }
}

void GrandMap::PCA()
{
    alglib::real_2d_array data;
    ProjectionView* pview;
    data.setlength(projectionview_pool->size(),9);
    for(int i = 0; i < projectionview_pool->size(); i++){
        pview = projectionview_pool->at(i);
        data[i][0] = pview->getOutlying();
        data[i][1] = pview->getSkewed();
        data[i][2] = pview->getClumpy();
        data[i][3] = pview->getSparse();
        data[i][4] = pview->getStriated();
        data[i][5] = pview->getConvex();
        data[i][6] = pview->getSkinny();
        data[i][7] = pview->getStringy();
        data[i][8] = pview->getMonotonic();
    }
    alglib::ae_int_t npoints = projectionview_pool->size();
    alglib::ae_int_t nvars = 9;
    alglib::ae_int_t info;
    alglib::real_1d_array s2;
    alglib::real_2d_array V;
    alglib::pcabuildbasis(data, npoints, nvars, info, s2, V);

    for(int r = 0; r < V.rows(); r++){
        xp->push_back(V[r][0]);
        yp->push_back(V[r][1]);
    }
}

float GrandMap::projectionhelper(ProjectionView *p, std::vector<float> *pv)
{
    float value = 0;
    value +=( p->getOutlying() * pv->at(0));
    value +=( p->getSkewed() * pv->at(1));
    value +=( p->getClumpy() * pv->at(2));
    value +=( p->getSparse() * pv->at(3));
    value +=( p->getStriated() * pv->at(4));
    value +=( p->getConvex() * pv->at(5));
    value +=( p->getSkinny() * pv->at(6));
    value +=( p->getStringy() * pv->at(7));
    value +=( p->getMonotonic() * pv->at(8));
    return value;
}

void GrandMap::init_Max_Min()
{
    ProjectionView* pview;
    float x = 0;
    float y = 0;
    if(projectionview_pool->size() > 0){
        pview = projectionview_pool->at(0);
        this->max_x = this->projectionhelper(pview, xp);
        this->min_x = this->max_x;
        this->max_y = this->projectionhelper(pview, yp);
        this->min_y = this->max_y;
        projectionview_pool->at(0)->setX(max_x);
        projectionview_pool->at(0)->setY(max_y);
    }

    for(int p = 1; p < projectionview_pool->size(); p++){
        pview = projectionview_pool->at(p);
        x = this->projectionhelper(pview, xp);
        if(x > this->max_x)
            this->max_x = x;
        if(x < this->min_x)
            this->min_x = x;
        pview->setX(x);

        y = this->projectionhelper(pview, yp);
        if(y > this->max_y)
            this->max_y = y;
        if(y < this->min_y)
            this->min_y = y;

        pview->setY(y);
    }
}

void GrandMap::targetProjectionPursuit(float mx, float my)
{
    Eigen::MatrixXf T; // target view matrix
    Eigen::MatrixXf S; // data source matrix
    Eigen::MatrixXf U;
    Eigen::MatrixXf V;

    float lx;
    float ly;
    float valuex;
    float valuey;

    alglib::real_2d_array a;
    alglib::ae_int_t m = 9;
    alglib::ae_int_t n = 2;
    alglib::ae_int_t uneeded  = 1;
    alglib::ae_int_t vtneeded = 1;
    alglib::ae_int_t additionalmemory = 0;
    alglib::real_1d_array w;
    alglib::real_2d_array u;
    alglib::real_2d_array vt;
    bool rs;
    ProjectionView* pview;


    //fill out the source matrix and the target matrix
    int rows = this->projectionview_pool->size();
    T.resize(rows, 2);
    S.resize(rows, 9);
    for(int r = 0; r < rows; r++){
      pview = this->projectionview_pool->at(r);
      lx = pview->getLocationX() + mx;
      ly = pview->getLocationY() + my;
      valuex = this->back_rescaleX(lx);
      valuey = this->back_rescaleY(ly);
      T(0,r) = valuex;
      T(1,r) = valuey;
    }
}

void GrandMap::normalizaton()
{
    ProjectionView* pview;
    for(int p = 0; p < projectionview_pool->size(); p++){
        pview = projectionview_pool->at(p);
        pview->setLocationX((pview->getX() - this->min_x)/(this->max_x - this->min_x)*this->width
                            + this->x);
        pview->setLocationY((pview->getY() -this->min_y)/(this->max_y - this->min_y)*this->height
                            + this->y);
    }
}

std::vector<std::vector<float> *> *GrandMap::GT_At_Random()
{
    int d = this->datasource->getDimension();
    std::vector<float>* vx = new std::vector<float>();
    std::vector<float>* vy = new std::vector<float>();
    std::vector<std::vector<float> *> * v =new std::vector<std::vector<float> *>();
    v->push_back(vx);
    v->push_back(vy);
    Eigen::VectorXf X = randomVector(d);
    Eigen::VectorXf Y = randomVector(d);
    while(Y[0] == X[0] && X[1] == Y[1]){
        Y = randomVector(d);
    }
    Y = Y - X.dot(Y) * X;
    Y = Y/Y.norm();

    for(int i = 0; i<X.rows(); i++){
        vx->push_back(X[i]);
        vy->push_back(Y[i]);
    }
    return v;
}

Eigen::VectorXf GrandMap::randomVector(int d)
{
    srand (time(0));
    Eigen::VectorXf P(d);
    double r, sum;
    do{
        sum = 0;
        for(int i = 0; i < d; i++){
            r = 2*((double) rand() / (RAND_MAX))-1;
            sum += r*r;
            P[i] = r;
        }
    }while(sum > 1);
    return P/P.norm();
}

std::vector<std::vector<float> *> *GrandMap::WegmanAlgorithm(float degree)
{
    int dimension = this->datasource->getDimension();
    std::vector<float>* vx = new std::vector<float>();
    std::vector<float>* vy = new std::vector<float>();
    std::vector<std::vector<float> *> * v =new std::vector<std::vector<float> *>();
    v->push_back(vx);
    v->push_back(vy);
    for(int d = 1; d <= dimension; d++){
        if(d%2 != 0){
            vx->push_back(sqrt(2.0/dimension)*sin(floor(d/2.0)*degree/M_PI));
            vy->push_back(sqrt(2.0/dimension)*cos(floor(d/2.0)*degree/M_PI));
        }else{
            vx->push_back(sqrt(2.0/dimension)*cos(floor(d/2.0)*degree/M_PI));
            vy->push_back(-sqrt(2.0/dimension)*sin(floor(d/2.0)*degree/M_PI));
        }
    }
    return v;
}

ProjectionView *GrandMap::get_PCA_Projectionview()
{
    alglib::real_2d_array data;
    ProjectionView * pview;
    std::vector<float>* xp = new std::vector<float>();
    std::vector<float>* yp = new std::vector<float>();
    int rows = this->datasource->getDataEntrys();
    int cols = this->datasource->getDimension();
    data.setlength(rows, cols);

    for(int r = 0; r < rows; r++)
        for(int c = 0; c < cols; c++)
            data[r][c] = this->datasource->getItem(r,c);

    alglib::ae_int_t npoints = rows;
    alglib::ae_int_t nvars = cols;
    alglib::ae_int_t info;
    alglib::real_1d_array s2;
    alglib::real_2d_array v;
    alglib::pcabuildbasis(data, npoints, nvars, info, s2,v );

    for(int r = 0; r < v.rows(); r++){
        xp->push_back(v[r][0]);
        yp->push_back(v[r][1]);
    }

    pview = new ProjectionView(xp, yp, this->datasource);
    pview->setColor(9);// 1 represent PCA color
    return pview;

}

void GrandMap::KMean(int k)
{
    int n = projectionview_pool->size();
    int d = this->xp->size();
    Scalar points[n*d];
    for(int i = 0; i < n; i++){
        points[i*d+0] = projectionview_pool->at(i)->getOutlying();
        points[i*d+1] = projectionview_pool->at(i)->getSkewed();
        points[i*d+2] = projectionview_pool->at(i)->getClumpy();
        points[i*d+3] = projectionview_pool->at(i)->getSparse();
        points[i*d+4] = projectionview_pool->at(i)->getStriated();
        points[i*d+5] = projectionview_pool->at(i)->getConvex();
        points[i*d+6] = projectionview_pool->at(i)->getSkinny();
        points[i*d+7] = projectionview_pool->at(i)->getStringy();
        points[i*d+8] = projectionview_pool->at(i)->getMonotonic();

    }
    Scalar centers[k*d];
    int assignments[n];
    RunKMeansPlusPlus(n, k, d, points, 5, centers, assignments);

    for(int i = 0; i < n; i++){
        projectionview_pool->at(i)->setColor(assignments[i]);
    }

}

float GrandMap::ChordalDistance(Eigen::MatrixXf &basesA, Eigen::MatrixXf &basesB)
{
    //Chordal distance
    //using frobenius norm and AA_t - BB_t
    Eigen::MatrixXf chordalMat = basesA * basesA.transpose() - basesB * basesB.transpose();
    // don't need the const multiplier 1/sqrt(2)
    //return chordalMat.norm();
    return CHORDAL_DIST_MULTIPLIER*chordalMat.norm();
}

void GrandMap::ProjectionDistanceDistributionCalculation()
{
    std::vector<float>* xp1;
    std::vector<float>* yp1;
    std::vector<float>* xp2;
    std::vector<float>* yp2;
    float result;
    float max;
    int d = this->datasource->getDimension();
    Eigen::MatrixXf m(d, 2);
    Eigen::MatrixXf n(d, 2);

    for(int i = 0; i < projectionview_pool->size(); i++){
        xp1 = projectionview_pool->at(i)->getXP();
        yp1 = projectionview_pool->at(i)->getYP();
        max = 0;
        for(int j = 0; j < projectionview_pool->size(); j++){
            if(i ==  j) continue;
            xp2 = projectionview_pool->at(j)->getXP();
            yp2 = projectionview_pool->at(j)->getYP();

            for(int k = 0; k < xp1->size(); k++){
                m(k, 0) = xp1->at(k);
                m(k, 1) = yp1->at(k);
                n(k, 0) = xp2->at(k);
                n(k, 1) = yp2->at(k);
            }
            result = this->ChordalDistance(m, n);
            if(max < result)
                max = result;
        }
        distance->push_back(max);
    }
}


float GrandMap::getProjectionEuclideanDistance(ProjectionView *p1, ProjectionView *p2)
{
    float sum = 0;
    sum += pow(p1->getOutlying() - p2->getOutlying(), 2);
    sum += pow(p1->getClumpy() - p2->getClumpy(), 2);
    sum += pow(p1->getConvex() - p2->getConvex(), 2);
    sum += pow(p1->getSkewed() - p2->getSkewed(), 2);
    sum += pow(p1->getStriated() - p2->getStriated(), 2);
    sum += pow(p1->getStringy() - p2->getStringy(), 2);
    sum += pow(p1->getMonotonic() - p2->getMonotonic(), 2);
    sum += pow(p1->getSkinny() - p2->getSkinny(), 2);
    sum += pow(p1->getSparse() - p2->getSparse(), 2);

    return sum;
}

void GrandMap::generateRepresentitiveProjectionView()
{
    /*this->candidate_projectionview->clear();
    int size = this->projectionview_pool->size();
    ProjectionView* pview;
    bool flag;
    this->candidate_projectionview->push_back(this->projectionview_pool->at(0));
    for(int i = 1; i < size; i++){
        flag = true;
        pview = this->projectionview_pool->at(i);
        for(int i = 0; i < this->candidate_projectionview->size(); i++){
            if(this->getProjectionEuclideanDistance(pview, this->candidate_projectionview->at(i)) < distance){
                flag = false;
                break;
            }
        }
       if(flag)
            this->candidate_projectionview->push_back(pview);
    }*/

}

float GrandMap::back_rescaleX(float x)
{
    return (x - this->x) * (this->max_x - this->min_x)/this->width + this->min_x;
}

float GrandMap::back_rescaleY(float y)
{
    return (y - this->y) * (this->max_y - this->min_y)/this->height + this->min_y;
}

void GrandMap::outputMetricsValue()
{
    labels.append("outlying");
    labels.append("skewed");
    labels.append("clumpy");
    labels.append("sparse");
    labels.append("striated");
    labels.append("convex");
    labels.append("skinny");
    labels.append("stringy");
    labels.append("monotonic");
    ProjectionView* view;
    QString filename="/Users/lizhimin/Desktop/Data.csv";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << "outlying,";
        stream << "skewed,";
        stream << "clumpy,";
        stream << "sparse,";
        stream << "striated,";
        stream << "convex,";
        stream << "skinny,";
        stream << "stringy,";
        stream << "monotonic" << endl;
        for(int i = 0; i < projectionview_pool->size(); i++){
            view = projectionview_pool->at(i);
            stream << view->getOutlying() << ",";
            stream << view->getSkewed() << ",";
            stream << view->getClumpy() << ",";
            stream << view->getSparse() << ",";
            stream << view->getStriated() << ",";
            stream << view->getConvex() << ",";
            stream << view->getSkinny() << ",";
            stream << view->getStringy() << ",";
            stream << view->getMonotonic() << "," << endl;
        }
    }

}

