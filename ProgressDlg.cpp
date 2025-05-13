#include "ProgressDlg.h"
#include "ui_ProgressDlg.h"

CProgressDlg::CProgressDlg(QWidget *parent) : QDialog(parent), ui(new Ui::CProgressDlg)
{
    ui->setupUi(this);

    BackupWasCanceled = false;
    ui->Progress_pbar->setRange(0, 100);
    ui->Progress_pbar->setValue(0);
}

CProgressDlg::~CProgressDlg()
{
    delete ui;
}

void CProgressDlg::SetFileCount(uint32 count, uint32 total)
{
    QString outstr;

    outstr = QString::number(count) + " of " + QString::number(total);
    ui->FileCnt_lbl->setText(outstr);
    ui->Progress_pbar->setValue(count * 100 / total);
}

void CProgressDlg::on_Cancel_btn_clicked()
{
    BackupWasCanceled = true;
}
