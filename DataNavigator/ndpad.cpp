#include "ndpad.h"
#include <math.h>
#include <QDebug>
NDPad::NDPad(float x, float y, QStringList *labels, Storage *datasource)
{
    this->x = x;
    this->y = y;
    this->dimension = labels->size();
    this->labels = labels;
    this->radius = 90;
    this->gap = 20;
    this->flag = true;
    this->datasource = datasource;
    for(int i = 0; i < dimension; i++){
        int px, py;
        px = this->radius * cos(M_PI * 2 * i / dimension) + x;
        py = this->radius * sin(M_PI * 2 * i / dimension) + y;
        polyX << QPoint(px, py);

        px = (this->radius + 20) * cos(M_PI * 2 * i / dimension) + x;
        py = (this->radius + 20) * sin(M_PI * 2 * i / dimension) + y;
        polyY << QPoint(px, py);
    }
    this->circleX.setRect(x-5, y-5, 10, 10);
    this->circleY.setRect(x-5, y-5, 10, 10);
}

NDPad::~NDPad()
{

}

void NDPad::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush white(Qt::white);
    QBrush blue(Qt::green);
    QBrush red(Qt::yellow);
    QPen textPen(Qt::black);

    painter->setPen(textPen);
    painter->setBrush(white);

    painter->drawPolygon(polyX);
    painter->setBrush(blue);
    float y2 = 0;
    float x2 = 0;
    for(int i = 0; i < dimension; i++){
        x2 = (this->radius + this->gap) * cos(M_PI * 2 * i / dimension)
                + x - 15;
        y2 = (this->radius + this->gap) * sin(M_PI * 2 * i / dimension)
                + y + 5;

        painter->setPen(textPen);
        painter->drawText(x2, y2, labels->at(i));
    }
    QRect* rect;
    QBrush brush(Qt::blue);
    QColor c;
    int r, g, b;
    for(int i = 0; i < padPoints.size(); i++){
        this->getColor(padPointsValue.at(i), &r, &g, &b);
        c.setRgb(r, g, b);
        brush.setColor(c);
        rect = padPoints.at(i);
        painter->fillRect(rect->topLeft().x(), rect->topLeft().y(), rect->width(), rect->height(), brush);
    }

    painter->setBrush(blue);
    painter->drawRect(circleY);
    painter->setBrush(red);
    painter->drawRect(circleX);
}

void NDPad::mouseReleaseEvent(QMouseEvent *event)
{

}

void NDPad::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = event->pos();
    Eigen::VectorXf V, W;
    if(isInPad(pos)){
        //check which circle is active
        if(sqrt(pow(pos.x() - this->circleX.center().x(), 2) + pow(pos.y() - this->circleX.center().y(), 2)) < 7){
              flag = true;
              this->circleX.moveTo(pos.x() - 10, pos.y() - 10);
              V = this->get_X_Vector(pos.x(), pos.y());
              W = this->getOrthogonal_Y_Vector();
              this->plot->setProjection(V, W);
              this->info->setProjectionView(V, W);
        }
        else if(sqrt(pow(pos.x() - this->circleY.center().x(), 2) + pow(pos.y() - this->circleY.center().y(), 2)) < 7){
              flag = false;
              this->circleY.moveTo(pos.x() - 10, pos.y() - 10);
              this->get_Y_Vector(pos.x(), pos.y());
              V = this->getOrthogonal_Y_Vector();
              this->plot->setProjection(x_axis, V);
              this->info->setProjectionView(x_axis, V);
        }
    }
}

void NDPad::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = event->pos();
    if(isInPad(pos)){
        if(flag){
            this->circleX.moveTo(pos.x() - 5, pos.y() - 5);
        }else{
            this->circleY.moveTo(pos.x() - 5, pos.y() - 5);
        }
    }
    this->mousePressEvent(event);
}

void NDPad::setPlot(ScatterPlot *plot)
{
    this->plot = plot;
}

void NDPad::setInfo(ScatterPlotInfoPanel *info)
{
    this->info = info;
}

void NDPad::setInitalProjection(std::vector<float> *xp, std::vector<float> *yp)
{
    Eigen::VectorXf xa(xp->size());
    Eigen::VectorXf ya(xp->size());

    for(int i = 0; i < dimension; i++){
        xa[i] = xp->at(i);
        ya[i] = yp->at(i);
    }
    this->x_axis = xa;
    this->X_AXIS = xa;
    this->y_axis = ya;
    this->Y_AXIS = ya;
    //make circle back to the center
    this->circleX.setRect(x-5, y-5, 10, 10);
    this->circleY.setRect(x-5, y-5, 10, 10);

    this->colorGuiderInitial();
}

void NDPad::resetPad()
{
    this->X_AXIS = this->x_axis;
    this->Y_AXIS = this->y_axis;

    this->circleX.setRect(x-5, y-5, 10, 10);
    this->circleY.setRect(x-5, y-5, 10, 10);
    this->colorGuiderInitial();
}

QRectF NDPad::boundingRect() const
{

}

bool NDPad::isInPad(QPointF p)
{
    float distance = sqrt(pow(p.x() - this->x, 2) + pow(p.y() - this->y, 2));
    if(distance > radius - this->circleX.width()/3)
        return false;
    else
        return true;
}

float NDPad::getWeightHelper(QPointF loc, QPointF p1, QPointF p2, QPointF p3)
{
    float c = sqrt(pow(loc.x() - p1.x(), 2)+pow(loc.y() - p1.y(), 2));
    float a = sqrt(pow(p1.x() - p2.x(), 2)+pow(p1.y() - p2.y(), 2));
    float b = sqrt(pow(loc.x() - p2.x(), 2)+pow(loc.y() - p2.y(), 2));
    float alpha = -(pow(c, 2) - pow(a, 2) - pow(b, 2))/(2*a*b);
    alpha = alpha/sqrt(1 - pow(alpha, 2));
    c = sqrt(pow(loc.x() - p3.x(), 2)+pow(loc.y() - p3.y(), 2));
    a = sqrt(pow(p3.x() - p2.x(), 2)+pow(p3.y() - p2.y(), 2));
    b = sqrt(pow(loc.x() - p2.x(), 2)+pow(loc.y() - p2.y(), 2));
    float beta = -(pow(c, 2) - pow(a, 2) - pow(b, 2))/(2*a*b);
    beta = beta/sqrt(1 - pow(beta, 2));

    if(alpha+beta == -INFINITY)
        return 10;

    return (alpha + beta)/(b*b);
}

void NDPad::colorGuiderInitial()
{

    float startX, startY, rw, rh;
    int row, col;
    QRect* rect;

    startX = this->x - this->radius;
    startY = this->y - this->radius;
    rw = 20;
    rh = 20;
    row= 2 * this->radius / rw;
    col= 2 * this->radius / rh;
    padPoints.clear();
    padPointsValue.clear();
    for(int r = 0; r < row; r++){
        for(int c = 0; c < col; c++){
            QPointF p(startX + r * rw + rw/2, startY + c * rh + rh/2);
            if(this->isInPad(p)){
                rect = new QRect(startX + r * rw, startY + c * rh, rw, rh);
                padPoints.append(rect);
                Eigen::VectorXf XAXIS = this->get_X_Vector(p.x(), p.y());
                Eigen::VectorXf YAXIS = this->getOrthogonal_Y_Vector();
                ProjectionView projection(XAXIS, YAXIS, this->datasource);
                padPointsValue.append(projection.getMonotonic());
            }
        }
        qDebug() << "row:" << r;
    }
    qDebug() << "number of rect" << padPoints.size();
    this->x_axis = this->X_AXIS;
    this->y_axis = this->Y_AXIS;
}

void NDPad::getColor(float value, int *red, int *green, int *blue)
{
    int aR = 0;   int aG = 0; int aB=255;  // RGB for our 1st color (blue in this case).
    int bR = 255; int bG = 0; int bB=0;    // RGB for our 2nd color (red in this case).

    if(value > 1) value = 1;

    *red   = (int)((bR - aR) * value) + aR;      // Evaluated as -255*value + 255.
    *green = (int)((bG - aG) * value) + aG;      // Evaluates as 0.
    *blue  = (int)((bB - aB) * value) + aB;      // Evaluates as 255*value + 0.
}

Eigen::VectorXf NDPad::getBarycentricCoordinateVector(float x, float y)
{
    QVector<float> Tvector, WeightVector;
    Eigen::VectorXf result;
    QPointF loc(x, y);
    QList<QPoint> points;
    for(int i = 0; i < dimension; i++)
    {
        float x1 = (float)(radius * cos(M_PI * 2 * i / dimension)+ this->x);
        float y1 = (float)(radius * sin(M_PI * 2 * i / dimension) +this->y);
        QPoint point(x1, y1);
        points.append(point);
    }

    for(int i = 0; i < dimension; i++){
        Tvector.append(getWeightHelper(loc, points.at(i), points.at((i+1)%(dimension)), points.at((i+2)%(dimension))));
    }

    int size = Tvector.size();
    float sum = 0;
    for(int i = 0; i < size; i++)
        sum += (Tvector.at(i)*Tvector.at(i));
    sum = sqrt(sum);

    for(int i = 0; i < size; i++){
        // the weight vector calculate we start from index 1 instead of 0
        // index 0 will be at the end, here we recover it's place
        if( i == size-1)
            WeightVector.push_front(Tvector.at(i)/sum);
        else
            WeightVector.push_back(Tvector.at(i)/sum);
    }

    result.resize(WeightVector.size());
    for(int i = 0; i < WeightVector.size(); i++)
        result[i] = WeightVector.at(i);

    return result;
}

Eigen::VectorXf NDPad::get_Y_Vector(float x, float y)
{
    Eigen::VectorXf result = this->getBarycentricCoordinateVector(x, y);
    for(int i = 0; i < result.rows(); i++){
        this->y_axis[i] = this->Y_AXIS[i] * result[i];
    }
    this->y_axis = this->y_axis/this->y_axis.norm();
    return this->y_axis;
}

Eigen::VectorXf NDPad::getOrthogonal_X_Vector()
{
    Eigen::VectorXf PX;
    PX = this->Orthogonal(y_axis, x_axis);
    x_axis = PX;
    return PX;
}

Eigen::VectorXf NDPad::get_X_Vector(float x, float y)
{
    Eigen::VectorXf result = this->getBarycentricCoordinateVector(x, y);
    for(int i = 0; i < result.rows(); i++){
        this->x_axis[i] = this->X_AXIS[i] * result[i];
    }
    this->x_axis = this->x_axis/this->x_axis.norm();
    return this->x_axis;
}

Eigen::VectorXf NDPad::getOrthogonal_Y_Vector()
{
    Eigen::VectorXf PY;
    PY = this->Orthogonal(x_axis, y_axis);
    y_axis = PY;
    return PY;
}

Eigen::VectorXf NDPad::Orthogonal(Eigen::VectorXf PX, Eigen::VectorXf PY)
{
    Eigen::VectorXf TP;
    TP = PY - PX.dot(PY) * PX;
    return TP/TP.norm();
}
