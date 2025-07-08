#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
#include <algorithm>
#include <qdebug.h>

struct Person
{
    QString name="New Person";
    QString Job="";
    qint16 salary=0;
    friend QDebug operator<<(QDebug debug, const Person &P);
};

template <class T>
class DataBase
{
    T *M;
    int size;
public:
    DataBase() : size(0), M(nullptr) {}
    ~DataBase() {delete[] M;}
    void push(T elem)
    {
        int TmpSize = size + 1;
        T *TmpArr = new T[TmpSize];
        for (int i = 0; i < size; i++)
        {
            TmpArr[i] = M[i];
        }
        TmpArr[size] = elem;
        delete[] M;
        M = TmpArr;
        size = TmpSize;
    }

    void pop()
    {
        int TmpSize = size - 1;
        T *TmpArr = new T[TmpSize];
        for (int i = 0; i < TmpSize; i++)
        {
            TmpArr[i] = M[i];
        }
        delete[] M;
        M = TmpArr;
        size = TmpSize;
    }
    void show()
    {
        for (int i =0; i < size; i++)
        {
            qDebug() << M[i];
        }
    }

    int get_size() const {return size;}

    T& operator[](const int index)
    {
        if (index < size) return M[index];
        else throw std::exception();
    }   
};

#endif // DATABASE_H
