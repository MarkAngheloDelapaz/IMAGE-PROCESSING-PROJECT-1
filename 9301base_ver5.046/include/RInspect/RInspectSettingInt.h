#ifndef RINSPECTSETTINGINT_H
#define RINSPECTSETTINGINT_H

#include "RInspectSettingItem.h"

class RInspectSettingInt : public RInspectSettingItem
{
public:
	explicit RInspectSettingInt(RDataItem *item);

	// RInspectSettingItem interface

public:
//	virtual RDataItem::ALLOWED_TYPE GetType();
	// RInspectSettingItem interface
public:
	virtual void DrawMenuLabel(int pos_x, int pos_y);
	virtual int GetMenuButton(button_t *btn, int pos_x, int pos_y);

private:

	// RInspectSettingItem interface
public:
	virtual int GetMenuSizeY();
	virtual int GetMenuSizeX();
	virtual int GetMenuButtonNum();
	virtual int ClickMenuButton(int num);
	
#ifdef QT_CORE_LIB
	virtual RInspectFrame *CreateFrame(RDataItem *item, RDialogSetting *dlg);
#endif
};

#endif // RINSPECTSETTINGINT_H
