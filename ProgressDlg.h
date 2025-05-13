#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <QDialog>

#include "PndDefs.h"

namespace Ui {
class CProgressDlg;
}

class CProgressDlg : public QDialog
{
    Q_OBJECT

public:
    bool BackupWasCanceled;

    explicit CProgressDlg(QWidget *parent = nullptr);
    ~CProgressDlg();

    void SetFileCount(uint32 count, uint total);

private slots:
    void on_Cancel_btn_clicked();

private:
    Ui::CProgressDlg *ui;
};

#endif // PROGRESSDLG_H
