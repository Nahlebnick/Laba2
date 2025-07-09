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

QDebug operator<<(QDebug debug, const Person &P)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << P.name << P.Job << P.salary;
    return debug;
}

bool operator<(const Person& p1, const Person& p2)
{
    int x = QString::compare(p1.name, p2.name, Qt::CaseInsensitive);
    if (x < 0) return true;
    return false;
}
