#ifndef FILELOADER_H
#define FILELOADER_H
#include <QString>
#include <QList>
#include "storage.h"
class FileLoader
{
    /************************************************************
      This file is used to load the different type of file and
      store those data in the container. so far the .csv file is
      the only file can be parse by this class.

      This class will generate a storage class and provide a reference
      to this object
    ************************************************************/

public:
    FileLoader(QString path);

    Storage* getDataSource();

    QStringList* getLabels();

private:
    /*
     process_line() is a function used to process a single data
     from the data file*/
    std::vector<float>* process_line(QString line);
    void setLabels(QString line);

    /*
     container is a data pool which hold all the initial data
     */
     Storage* datasource;
    /*
     labels contain the information of the attribute name.
     */
     QStringList* labels;

    /*
     * the dimension of the data source
     */
    int dimension;
};

#endif // FILELOADER_H
