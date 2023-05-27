#ifndef RDIALOGSETTING_H
#define RDIALOGSETTING_H

#include <string>

#include "RInspectPlane.h"
#include "RDataList/RDataItem.h"
#include "RDataList/RDataListModel.h"

#include <QDialog>
#include <QFrame>

using std::string;

class RInspect;
class RInspectFrame;
class RInspectViewTeachItem;
class RInspectSettingItem;

namespace Ui {
class RDialogSetting;
}

class RDialogSetting : public QDialog
{
	Q_OBJECT

public:
	explicit RDialogSetting(RInspect *insp, RInspectPlain *scene, RDataItem *settings, RDataItem *results, AUTH_FLAGS settingflags = AUTH_FLAGS_EVERYONE, QWidget *parent = 0);
	~RDialogSetting();

	void SetScene(RInspectPlain *scene);
	void SetTeachItem(RInspectViewTeachItem *item);
	void SetSettingItem(RInspectSettingItem *item);
	void SetAuthority(AUTH_FLAGS flags);
	void SetText(string str);
	void ClearText();

	void RefleshGrayImage(bool checked);
	void RefleshBinImage(bool checked);

	void AddSettingFrame(RInspectFrame *frame);
	void RemoveSettingFrame(RInspectFrame *frame);

private slots:
	void on_btnTest_clicked();

	void on_cmbGray_currentIndexChanged(int index);

	void on_chkGray_clicked(bool checked);

	void on_chkBin_clicked(bool checked);

	void on_cmbBin_currentIndexChanged(int index);

	void SetMousePoint(QPointF p);

	void onEvintHorizontalSliderSetvalue(qreal value);

	void on_sldScale_valueChanged(int value);

	void on_chkFull_toggled(bool checked);

public slots:
	void RefleshDialog();

private:
	Ui::RDialogSetting *ui;

	RInspect *_insp;

	RInspectPlain *_scene;

	RDataItem *_settings;
	RDataItem *_results;

	vector<RInspectFrame *> _usermenu;


	RDataListModel *_modelsettings;
	RDataListModel *_modelresults;

	int _isfull;

	void _setParentFullScreen(bool isfull, QObject *obj);
};

#endif // RDIALOGSETTING_H
