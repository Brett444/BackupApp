#include "ProgressDlg.h"
#include "ui_ProgressDlg.h"

CProgressDlg::CProgressDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CProgressDlg)
{
    ui->setupUi(this);
}

CProgressDlg::~CProgressDlg()
{
    delete ui;
}
