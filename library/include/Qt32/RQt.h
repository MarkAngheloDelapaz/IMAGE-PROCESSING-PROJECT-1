#ifndef RQT_H
#define RQT_H

#include <QWidget>

QWidget* RQtMainWindowWidget();
int RQtExecuteDialog(QDialog* dialog, int* result = NULL);

#endif // RQT_H
