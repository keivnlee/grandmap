#include "Storage.h"
#include <float.h>
Storage::Storage()
{
    this->datapool = new std::vector< std::vector<float>* >();
}

void Storage::addDataEntry(std::vector<float> *v)
{
    this->datapool->push_back(v);
}

std::vector<float> *Storage::getColumn(int c)
{
    std::vector<float>* columnData = new std::vector<float>();
    int entrys = this->getDataEntrys();
    for(int e = 0; e < entrys; e++)
        columnData->push_back(datapool->at(e)->at(c));
    return columnData;
}

std::vector<float> *Storage::getRow(int r)
{
    return datapool->at(r);
}

float Storage::getItem(int r, int c)
{
    return datapool->at(r)->at(c);
}

void Storage::normalization()
{
    int dimension = this->getDimension();
    int entrys =  this->getDataEntrys();
    float max = FLT_MIN;
    float min = FLT_MAX;
    float item = 0;
    for(int d = 0; d < dimension; d++){
        max = FLT_MIN;
        min = FLT_MAX;
        for(int e = 0; e < entrys; e++){
            item = datapool->at(e)->at(d);
            if(max < item) max = item;
            if(min > item) min = item;
        }

        for(int e = 0; e < entrys; e++){
            item = datapool->at(e)->at(d);
            datapool->at(e)->at(d) = (item - min)/(max - min);
        }
    }
}

void Storage::setLabels(QStringList *list)
{
    this->labels = list;
}

int Storage::getDimension()
{
    return this->datapool->front()->size();
}

int Storage::getDataEntrys()
{
    return this->datapool->size();
}

QStringList *Storage::getLabels()
{
    return this->labels;
}
