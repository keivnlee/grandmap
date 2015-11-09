#include "projectionpanel.h"
#include <QPainter>
#include <QApplication>
#include <QLayout>
#include <QDesktopWidget>

ProjectionPanel::ProjectionPanel(Storage *storage, ProjectionView *p, QWidget *parent) : QWidget(parent)
{
    attribute = new QLabel(tr("Attribute"));
    attributeBox = new QComboBox();
    panel = new PainterPanel(storage, p);

    QGridLayout *mainLayout = new QGridLayout;
    attributeBox->addItem(tr("attr1", "at1"));
    attributeBox->addItem(tr("attr2", "at2"));
    attribute->setBuddy(attributeBox);

    mainLayout->addWidget(panel, 0, 0, 1, 4);
    mainLayout->addWidget(attribute, 2, 0, Qt::AlignRight);
    mainLayout->addWidget(attributeBox, 2, 1);
    this->setLayout(mainLayout);

    QRect rect = QApplication::desktop()->screenGeometry();
    float width = rect.width() * 0.7;
    float height = rect.height() * 0.7;
    this->resize(width, height);
    this->setWindowTitle(tr("ND-Touch Pad"));
    //layout()->setSizeConstraint(QLayout::SetFixedSize);
}

ProjectionPanel::~ProjectionPanel()
{

}

void ProjectionPanel::setProjectionView(ProjectionView *p)
{
    panel->setProjectionView(p);
}

void ProjectionPanel::paintEvent(QPaintEvent *event)
{
}
