#include "window.h"
#include "ui_window.h"
#include "renderarea.h"
#include <QGridLayout>
#include <QMessageBox>
#include "DataNavigator/projectionpanel.h"
#include <QApplication>
#include <QDesktopWidget>
Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    renderArea = new RenderArea;
    SamplingBox = new QComboBox;
    SamplingBox->addItem(tr("Wegman"), 1);
    SamplingBox->addItem(tr("Grand tour Uniform"), 2);

    SamplingLabel = new QLabel(tr("&Sampling"));
    SamplingLabel->setBuddy(SamplingBox);

    FilteringBox = new QComboBox;
    FilteringBox->addItem(tr(""),"");
    FilteringBox->addItem(tr("K-Means"), "KM");

    FilteringLabel = new QLabel("Filtering");
    FilteringLabel->setBuddy(FilteringBox);

    KMeanBox = new QComboBox;
    KMeanBox->addItem(tr("2"), 2);
    KMeanBox->addItem(tr("3"), 3);
    KMeanBox->addItem(tr("4"), 4);
    KMeanBox->addItem(tr("5"), 5);
    KMeanBox->addItem(tr("6"), 6);
    KMeanBox->addItem(tr("7"), 7);
    KMeanBox->addItem(tr("8"), 8);
    KMeanLabel = new QLabel("K-Mean");
    KMeanLabel->setBuddy(KMeanBox);

    SampleNumberBox = new QComboBox;
    SampleNumberBox->addItem(tr("50"), 50);
    SampleNumberBox->addItem(tr("100"), 100);
    SampleNumberBox->addItem(tr("200"), 200);
    SampleNumberBox->addItem(tr("300"), 300);

    SampleNumberLabel = new QLabel("Smaple Number");
    SampleNumberLabel->setBuddy(SampleNumberBox);

    connect(SamplingBox, SIGNAL(activated(int)), this, SLOT(SamplingMethodChange()));
    connect(FilteringBox, SIGNAL(activated(int)), this, SLOT(FilteringMethodChange()));
    connect(SampleNumberBox, SIGNAL(activated(int)), this, SLOT(SampleNumberChange()));
    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    mainLayout->addWidget(SamplingLabel, 2, 0, Qt::AlignRight);
    mainLayout->addWidget(SamplingBox, 2, 1);
    mainLayout->addWidget(FilteringLabel, 3, 0, Qt::AlignRight);
    mainLayout->addWidget(FilteringBox, 3, 1);
    mainLayout->addWidget(KMeanBox, 3, 2);
    mainLayout->addWidget(SampleNumberLabel, 4, 0, Qt::AlignRight);
    mainLayout->addWidget(SampleNumberBox, 4, 1);
    setLayout(mainLayout);

    //set window size
    QRect rect = QApplication::desktop()->screenGeometry();
    float width = rect.width() * 0.8;
    float height = rect.height() * 0.7;
    this->resize(width, height);
    setWindowTitle(tr("GrandMap"));
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

Window::~Window()
{
    delete ui;
}

void Window::SamplingMethodChange()
{
    int index = SamplingBox->itemData(SamplingBox->currentIndex()).toInt();
    renderArea->SamplingMethodChange(index);

}

void Window::FilteringMethodChange()
{
    QString text = FilteringBox->itemData(FilteringBox->currentIndex()).toString();
    int k = KMeanBox->itemData(KMeanBox->currentIndex()).toInt();
    if(text == "")
        renderArea->FliteringMethodChange(0);
    else if(text == "KM")
        renderArea->FliteringMethodChange(1, k);

}

void Window::SampleNumberChange()
{
    int num = SampleNumberBox->itemData(SampleNumberBox->currentIndex()).toInt();
    renderArea->SampeNumberChange(num);
}


