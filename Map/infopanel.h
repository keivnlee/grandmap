#ifndef INFOPANEL_H
#define INFOPANEL_H
#include <QList>
#include <QString>
#include <QRect>
#include <QPainter>
#include <QGraphicsItem>
class InfoPanel : public QGraphicsItem
{
public:
    InfoPanel(float x, float y, float width, float height, std::vector<float> *X_axis, std::vector<float> *Y_axis, std::vector<float>* distance);
    ~InfoPanel();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;



private:

    int BarChar_ColumnIndex(float value);
    void drawProjectionDistanceDistributionBarChart(QPainter *painter);
    QList<QString> labels;
    std::vector<float>* distance;
    std::vector<float>* X_axis;
    std::vector<float>* Y_axis;

    //parameter for the bar chart
    float barX, barY, barWidth, barHeight, valueMin, valueMax;
    int* count;
    int barColumn;


    //panel position information
    float x, y, width, height;
};

#endif // INFOPANEL_H
