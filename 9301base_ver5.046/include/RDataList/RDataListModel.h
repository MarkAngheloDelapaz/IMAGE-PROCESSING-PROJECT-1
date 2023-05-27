#ifndef RMODELSETTINGS_H
#define RMODELSETTINGS_H

#include <QAbstractItemModel>

#include "RDataList/RDataItem.h"

class RDataListModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	RDataListModel(RDataItem *root, QObject *parent = 0);
	~RDataListModel();
	void append(vector<RDataItem *> list);
	void SetFlags(AUTH_FLAGS flag);

private:

	RDataItem *_items;
	AUTH_FLAGS _flags;

	// QAbstractItemModel interface
public:
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

};

#endif // RMODELSETTINGS_H
