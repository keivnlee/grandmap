#ifndef WINDOW_H
#define WINDOW_H
#include <QComboBox>
#include <QWidget>
#include <QLabel>
#include "renderarea.h"
namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();


private slots:
    void SamplingMethodChange();
    void FilteringMethodChange();
    void SampleNumberChange();

private:
    RenderArea *renderArea;

    QLabel *SamplingLabel;
    QLabel *FilteringLabel;
    QLabel *SampleNumberLabel;
    QLabel *KMeanLabel;
    QComboBox *SamplingBox;
    QComboBox *FilteringBox;
    QComboBox *SampleNumberBox;
    QComboBox *KMeanBox;

    Ui::Window *ui;
};

#endif // WINDOW_H
