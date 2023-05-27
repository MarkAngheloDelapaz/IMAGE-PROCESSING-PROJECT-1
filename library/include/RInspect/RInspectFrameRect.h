#ifndef RINSPECTFRAMERECT_H
#define RINSPECTFRAMERECT_H

#include "RInspect/RInspectFrame.h"
#include "RInspect/RDialogSetting.h"
#include "RInspect/RInspectFrameInt.h"

namespace Ui {
class RInspectFrameRect;
}

class RInspectFrameRect : public RInspectFrame
{
	Q_OBJECT

public:
	explicit RInspectFrameRect(RDataItem *item, RDialogSetting *parent = 0);
	~RInspectFrameRect();

private:
	Ui::RInspectFrameRect *ui;
	vector<RInspectFrameInt *> _rect;

public slots:
	virtual void updatevalue();
};

#endif // RINSPECTFRAMERECT_H
