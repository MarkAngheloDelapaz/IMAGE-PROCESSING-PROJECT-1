#ifndef RSETTINGDELEGATE_H
#define RSETTINGDELEGATE_H

#include <QItemDelegate>

class RDataListDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	explicit RDataListDelegate(QObject *parent = 0);

signals:

public slots:


	// QAbstractItemDelegate interface
public:
	virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // RSETTINGDELEGATE_H
