#ifndef PROJECTIONPANEL_H
#define PROJECTIONPANEL_H

#include <QWidget>
#include "scatterplot.h"
#include "ndpad.h"
#include "../Map/projectionview.h"
#include <QGraphicsItem>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "painterpanel.h"
class ProjectionPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ProjectionPanel(Storage* storage, ProjectionView *p, QWidget *parent = 0);
    ~ProjectionPanel();

    void setProjectionView(ProjectionView* p);

    void paintEvent(QPaintEvent *event);
public slots:

private slots:
    void attributeChange();
    void buttonClick();

private:
    PainterPanel *panel;
    QLabel *attribute;
    QComboBox *attributeBox;
    QPushButton *resetButton;
};

#endif // PROJECTIONPANEL_H
