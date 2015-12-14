#include "projectionpanel.h"
#include <QPainter>
#include <QApplication>
#include <QLayout>
#include <QDesktopWidget>

ProjectionPanel::ProjectionPanel(Storage *storage, ProjectionView *p, QWidget *parent) : QWidget(parent)
{
    attribute = new QLabel(tr("Attribute"));
    attributeBox = new QComboBox();
    resetButton = new QPushButton("reset", this);
    panel = new PainterPanel(storage, p);

    QGridLayout *mainLayout = new QGridLayout;

    QStringList* labels = storage->getLabels();
    for(int i = 0; i < labels->size(); i++){
        attributeBox->addItem(labels->at(i), i);
    }
    attribute->setBuddy(attributeBox);

    mainLayout->addWidget(panel, 0, 0, 1, 4);
    mainLayout->addWidget(attribute, 2, 0, Qt::AlignRight);
    mainLayout->addWidget(attributeBox, 2, 1);
    mainLayout->addWidget(resetButton, 2, 2);
    this->setLayout(mainLayout);

    connect(attributeBox, SIGNAL(activated(int)), this, SLOT(attributeChange()));
    connect(resetButton, SIGNAL(released()), this, SLOT(buttonClick()));
    QRect rect = QApplication::desktop()->screenGeometry();
    float width = rect.width() * 0.7;
    float height = rect.height() * 0.7;
    this->resize(width, height);
    this->setWindowTitle(tr("ND-Touch Pad"));
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

void ProjectionPanel::attributeChange()
{
    int num = attributeBox->itemData(attributeBox->currentIndex()).toInt();
    this->panel->attributeChange(num);
}

void ProjectionPanel::buttonClick()
{
    this->panel->buttonClick();
}
