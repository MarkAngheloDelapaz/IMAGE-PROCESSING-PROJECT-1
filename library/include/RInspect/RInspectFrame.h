#ifndef RINSPECTFRAME_H
#define RINSPECTFRAME_H

#include <QFrame>

#include <RDataList/RDataItem.h>
#include <RInspect/RDialogSetting.h>

namespace Ui {
class RInspectFrame;
}

class RInspectFrame : public QFrame
{
	Q_OBJECT

public:
	explicit RInspectFrame(RDataItem *item, RDialogSetting *parent = 0);
	~RInspectFrame();

public slots:
	virtual void updatevalue();

signals:
	void valuechanged();

private:
	Ui::RInspectFrame *ui;

};

#endif // RINSPECTFRAME_H
