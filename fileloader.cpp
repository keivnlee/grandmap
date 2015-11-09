#include<fileloader.h>
#include<QFile>
#include<QTextStream>
#include<QDebug>

FileLoader::FileLoader(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "error:no such file";
        return;
    }

    this->datasource = new Storage();
    this->labels = new QStringList();

    QTextStream in(&file);
    QStringList list = in.readLine().split("\r");
    setLabels(list.at(0));
    //first line is the dimension name
    //data will start to load from second line.
    datasource->setLabels(this->getLabels());
    for(int i = 1; i < list.size(); i++){
        datasource->addDataEntry(process_line(list.at(i)));
    }
    file.close();
    datasource->normalization();
}

Storage *FileLoader::getDataSource()
{
    return this->datasource;
}

QStringList *FileLoader::getLabels()
{
    return this->labels;
}


std::vector<float> *FileLoader::process_line(QString line){
    QStringList list = line.split(",");
    std::vector<float>* v = new std::vector<float>();

    //if the data is N/A in the file, we will set its value to zero.
    //-1 escape the name of the data.
    for(int i = 0; i < list.size(); i++){
        if(list.at(i) == "N/A")
            v->push_back(0);
        else
            v->push_back(list.at(i).toFloat());
    }
    return v;
}

void FileLoader::setLabels(QString line)
{
    QStringList list = line.split(",");
    for(int i = 0; i < list.size(); i++){
        QString label = list.at(i);
        label.remove("\"");
        this->labels->append(label);
    }
    this->dimension = this->labels->size();
}
