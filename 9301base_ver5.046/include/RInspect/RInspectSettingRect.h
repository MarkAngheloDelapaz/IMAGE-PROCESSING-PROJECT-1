#ifndef RINSPECTSETTIGRECT_H
#define RINSPECTSETTIGRECT_H

#include "RInspect/RInspectSettingItem.h"
#include "RInspect/RInspectSettingInt.h"

class RInspectSettingRect : public RInspectSettingItem
{
public:
	explicit RInspectSettingRect(RDataItem *item);

	void SetBasePoint(R_POINT base);

	// RInspectSettingItem interface
public:
	virtual void DrawMenuLabel(int pos_x, int pos_y);
	virtual int GetMenuButton(button_t *btn, int pos_x, int pos_y);
	virtual int ClickMenuButton(int num);

private:
	RInspectSettingInt *_items[4];
	R_POINT _base;


#ifdef QT_CORE_LIB
private:
	static RInspectFrame *temp;
	static RDataItem *tmpitem;
	static RDialogSetting *tmpdlg;
public:
	virtual RInspectFrame* CreateFrame(RDataItem *item, RDialogSetting *dlg);
signals:
	void changevalue();
#endif

};

#endif // RINSPECTSETTIGRECT_H
