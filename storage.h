#ifndef STORAGE_H
#define STORAGE_H
#include <vector>
#include <QStringList>
class Storage
{
    /****************************************************************
     storage is a class which is defined for storaging the data
     and provide more accessible way for other class to visit
     different part of the data.

     So far the storage class is only used to provide a method for storing
     matrix data.

     please reference to this website for further step
     http://www.eld.leidenuniv.nl/~moene/Home/tips/matrix2d/
     *****************************************************************/
public:
    Storage();
    ~Storage();
    void addDataEntry(std::vector<float>* v);

    /***************************************************************
     The following are the basic data access method help you to access
     different part of the data
     ***************************************************************/
     std::vector<float>* getColumn(int c);
     std::vector<float> *getRow(int r);
     float getItem(int r, int c);

     /***************************************************************
      The following method are provided to help you visite the basic
      information of the data
      ***************************************************************/
     int getDimension();
     int getDataEntrys();
     QStringList *getLabels();

    /************************************************************
     once the data is load, the range of each attribute
     in the data set will normalize into [0,1]
    ************************************************************/
    void normalization();

    void setLabels(QStringList* list);

private:
    std::vector< std::vector<float>* >* datapool;
    QStringList* labels;
};

#endif // STORAGE_H
