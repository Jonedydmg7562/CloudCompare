#ifndef CCDIDLG_H
#define CCDIDLG_H

#include "ui_DeleteIslandDialog.h"
#include <QDialog>

namespace Ui {
	class ccDIDlg;
}

class ccDIDlg : public QDialog, public Ui_Dialog
{
	Q_OBJECT

public:
	explicit ccDIDlg(QWidget* parent = 0);

protected slots:

	//! Saves (temporarily) the dialog paramters on acceptation
	void saveSettings();
};

#endif // CCSDIDLG_H
