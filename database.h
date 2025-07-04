#ifndef DATABASE_H
#define DATABASE_H
#include <QString>

struct Person
{
    QString name;
    QString Job;
    qint16 salary;
};

class DataBase
{
    int size;
public:
    DataBase();
};

#endif // DATABASE_H
