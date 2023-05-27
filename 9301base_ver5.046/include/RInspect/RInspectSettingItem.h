#ifndef RINSPECTSETTINGITEM_H
#define RINSPECTSETTINGITEM_H

#include <R_button.h>

#include "RDataList/RDataItem.h"

#ifdef QT_CORE_LIB
	#include "RInspect/RInspectFrame.h"
	#include "RInspect/RDialogSetting.h"
#endif

class RInspectSettingItem;
class RInspectFrame;

RInspectSettingItem *RInspectSettingCreateSettingItem(RDataItem *item);


#ifdef QT_CORE_LIB
class RInspectSettingItem : public QObject
{
	Q_OBJECT
#else
class RInspectSettingItem
{
#endif

public:
	explicit RInspectSettingItem(RDataItem *item);
	~RInspectSettingItem();

	RDataItem::ALLOWED_TYPE GetType();
	RDataItem *GetDataItem();

	virtual int GetMenuSizeY();
	virtual int GetMenuSizeX();
	virtual int GetMenuButtonNum();
	virtual void DrawMenuLabel(int pos_x, int pos_y);

	virtual int GetMenuButton(button_t *btn, int pos_x, int pos_y);
	virtual int ClickMenuButton(int num);


protected:
	RDataItem *_item;

	int _size_x;
	int _size_y;
	int _btn_num;

	static int _countBtnNum(button_t *btn);


#ifdef QT_CORE_LIB
public:
	RInspectFrame *GetRInspectFrame(RDialogSetting *dlg = NULL);

private:
	virtual RInspectFrame* CreateFrame(RDataItem *item, RDialogSetting *dlg);

signals:
	void changevalue();
#endif

};


#endif // RINSPECTSETTINGITEM_H
