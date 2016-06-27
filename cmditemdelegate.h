#ifndef CMDITEMDELEGATE_H
#define CMDITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class CmdItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CmdItemDelegate(QObject *parent = 0);
    static QString caption(const QString &text);
    static QString paragraph(const QString &text);
    static QString highlight(const QString &text);
public slots:
    void keywordChanged(QString keyword);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QString m_keyword;
};

#define S_CAPTION(t)	CmdItemDelegate::caption(t)
#define S_PARAGRAPH(t)	CmdItemDelegate::paragraph(t)
#define S_HIGHLIGHT(t)	CmdItemDelegate::highlight(t)

#endif // CMDITEMDELEGATE_H
