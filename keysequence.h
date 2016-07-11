#ifndef KEYSEQUENCE_H
#define KEYSEQUENCE_H

#include <QList>

template <class T>
class UniqueList : public QList<T>
{
public:
    inline UniqueList() : QList<T>() {}
    inline UniqueList(const QList<T> &other) : QList<T>(other) {}
    inline UniqueList(const UniqueList<T> &&other) : QList<T>(other) {}
    inline UniqueList(std::initializer_list<T> args) : QList<T>(args) {}
    inline bool contains(const UniqueList<T> &other) const
    {
        if (other.isEmpty())
            return true;

        for (int pos = 0; pos < size(); ++pos)
        {
            if ((pos = indexOf(other.first(), pos)) == -1)
                break;

            if (mid(pos, other.length()) == other)
                return true;
        }

        return false;

    }

//	inline UniqueList<T> operator+(const UniqueList<T> &other) const;
//    inline UniqueList<T> & operator+=(const UniqueList<T> &other);
//    inline UniqueList<T> & operator+=(const T &value);
};

typedef UniqueList<ulong> KeySequence;
#endif // KEYSEQUENCE_H
