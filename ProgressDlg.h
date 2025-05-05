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
    bool BackupWasCanceled, LogFails;

    explicit CProgressDlg(QWidget *parent = nullptr);
    ~CProgressDlg();

    void SetFileCount(uint32 count, uint total);

private slots:
    void on_Cancel_btn_clicked();

    void on_LogFiles_chk_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::CProgressDlg *ui;
};

#endif // PROGRESSDLG_H
