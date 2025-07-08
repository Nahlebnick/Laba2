#include "database.h"

QDebug operator<<(QDebug debug, const Person &P)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << P.name << P.Job << P.salary;
    return debug;
}
