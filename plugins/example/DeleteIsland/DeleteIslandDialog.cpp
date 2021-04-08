#include "DeleteIslandDialog.h"

//定义两个静态阈值，并初始化
static int threshold_1 = 3000;
static int threshold_2 = 3;
static double threshold_3 = 0.02;
static bool threshold_4 = true;

ccDIDlg::ccDIDlg(QWidget* parent) : QDialog(parent), Ui_Dialog()
{
	setupUi(this);

	//关联信号槽
	connect(okButton, SIGNAL(accepted()), this, SLOT(saveSettings()));

	//初始化设置阈值
	spinBox_1->setValue(threshold_1);
	spinBox_2->setValue(threshold_2);
	doubleSpinBox_1->setValue(threshold_3);
	spinBox_3->setValue(threshold_4);
}

void ccDIDlg::saveSettings()
{
	//OK后重新赋值
	threshold_1 = spinBox_1->value();
	threshold_2 = spinBox_2->value();
	threshold_3 = doubleSpinBox_1->value();
	threshold_4 = spinBox_3->value();
}
