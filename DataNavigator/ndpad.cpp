#include "ndpad.h"
#include <math.h>
NDPad::NDPad(float x, float y, QStringList *labels)
{
    this->x = x;
    this->y = y;
    this->dimension = labels->size();
    this->labels = labels;
    this->radius = 90;
    this->gap = 20;
    this->flag = true;

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
    QBrush blue(Qt::blue);
    QBrush white(Qt::white);
    QBrush red(Qt::red);
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
        painter->setBrush(blue);
        painter->drawEllipse(circleY);
        painter->setBrush(red);
        painter->drawEllipse(circleX);
    }
}

void NDPad::mouseReleaseEvent(QMouseEvent *event)
{

}

void NDPad::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = event->pos();
    Eigen::VectorXf V;
    if(isInPad(pos)){
        //check which circle is active
        if(sqrt(pow(pos.x() - this->circleX.center().x(), 2) + pow(pos.y() - this->circleX.center().y(), 2)) < 7){
              flag = true;
              this->circleX.moveTo(pos.x() - 10, pos.y() - 10);
              V = this->get_X_Vector(pos.x(), pos.y());
              this->plot->setProjection(V, this->getOrthogonal_Y_Vector());
        }
        else if(sqrt(pow(pos.x() - this->circleY.center().x(), 2) + pow(pos.y() - this->circleY.center().y(), 2)) < 7){
              flag = false;
              this->circleY.moveTo(pos.x() - 10, pos.y() - 10);
              V = this->get_Y_Vector(pos.x(), pos.y());
              this->plot->setProjection(x_axis, this->getOrthogonal_Y_Vector());
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
}

QRectF NDPad::boundingRect() const
{

}

bool NDPad::isInPad(QPointF p)
{
    float distance = sqrt(pow(p.x() - this->x, 2) + pow(p.y() - this->y, 2));
    if(distance > radius - this->circleX.width())
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
    PY = PY - PX.dot(PY) * PX;
    return PY/PY.norm();
}
