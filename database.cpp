#include "database.h"

QDataStream& operator<<(QDataStream& out, const Person &P)
{
    out << P.name;
    out << P.Job;
    out << P.salary;
    return out;
}

QDataStream& operator>>(QDataStream& in,  Person &P)
{
    in >> P.name;
    in >> P.Job;
    in >> P.salary;
    return in;
}
