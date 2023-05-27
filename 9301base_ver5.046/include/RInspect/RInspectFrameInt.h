#ifndef RINSPECTFRAMEINT_H
#define RINSPECTFRAMEINT_H

#include <QFrame>
#include <QIntValidator>
#include <QLineEdit>

#include "RDataList/RDataItem.h"
#include "RInspect/RInspectFrame.h"

namespace Ui {
class RInspectFrameInt;
}

class RInspectFrameInt : public RInspectFrame
{
	Q_OBJECT

public:
	explicit RInspectFrameInt(RDataItem *item, RDialogSetting *parent = 0);
	~RInspectFrameInt();

	// RInspectFrame interface
public slots:
	virtual void updatevalue();


private:
	Ui::RInspectFrameInt *ui;
	//QIntValidator *_validator;
	int *_value;
	int _def;

	void changeColorBy(QValidator::State state, QLineEdit *sender);

private slots:
	//void on_lineEdit_textChanged(const QString &arg1);

	void on_spinBox_valueChanged(int arg1);
};

#endif // RINSPECTFRAMEINT_H
