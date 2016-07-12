#ifndef KEYSEQUENCE_H
#define KEYSEQUENCE_H

#include <QList>

template <class T>
class UniqueList : public QList<T>
{
public:
    inline UniqueList() : QList<T>() {}
    inline UniqueList(const UniqueList<T> &other) : QList<T>(other) {}
    inline UniqueList(const UniqueList<T> &&other) : QList<T>(other) {}
    inline UniqueList(std::initializer_list<T> args) : QList<T>(args) {}
    inline UniqueList<T> operator+(const UniqueList<T> &other) const
    {
        UniqueList<T> result(*this);
        for (auto i : other) {
            if (!result.contains(i))
                result.append(i);
        }
        return result;
    }
    inline UniqueList<T> & operator+=(const UniqueList<T> &other)
    {
        for (auto i : other) {
            if (!this->contains(i))
                this->append(i);
        }
        return *this;
    }
    inline UniqueList<T> & operator+=(const T &value)
    {
        if (!this->contains(value))
            this->append(value);
        return *this;
    }
    inline UniqueList<T> & operator-=(const T &value)
    {
        this->removeOne(value);
        return *this;
    }
};

typedef UniqueList<ulong> KeySequence;
typedef QSet<ulong> KeyModifier;

#endif // KEYSEQUENCE_H
