#ifndef CMDITEMDELEGATE_H
#define CMDITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class CmdItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CmdItemDelegate(QObject *parent = 0);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // CMDITEMDELEGATE_H
