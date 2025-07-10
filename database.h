#ifndef DATABASE_H
#define DATABASE_H
#include <QString>
#include <algorithm>
#include <qdebug.h>
#include <QFile>
#include <QDataStream>

struct Person
{
    QString name="New Person";
    QString Job="";
    qint16 salary=0;
    friend QDataStream& operator<<(QDataStream& out, const Person &P);
    friend QDataStream& operator>>(QDataStream& in, Person &P);
    friend QDebug operator<<(QDebug debug, const Person &P);
    friend bool operator<(const Person&, const Person&);
};

template <class T>
class DataBase
{
    T *M;
    int size;
    int MagicNumber = 7615453;
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

    void clear()
    {
        if (size > 0)
        {
            delete[] M;
            M = nullptr;
        }
        size = 0;
    }

    void sort_db()
    {
        std::sort(M, M+size, [](T first, T second){return first < second;});
    }

    bool writeIntoFile(const QString& filename)
    {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Error";
            return false;
        }
        else
        {
            QDataStream out(&file);
            out << (quint32)MagicNumber;
            out << size;
            for (int i = 0; i < size; i++)
            {
                out << M[i];
            }
            file.close();
            return true;
        }
    }

    bool readFromFile(const QString& filename)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Error";
            return false;
        }
        else
        {
            delete[] M;
            QDataStream in(&file);
            quint32 magic;
            in >> magic;
            if (magic != MagicNumber) {
                return false;
            }
            in >> size;
            M = new T[size];
            for (int i = 0; i < size; i++)
            {
                in >> M[i];
            }
            file.close();
            return true;
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
