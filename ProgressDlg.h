#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <QDialog>

namespace Ui {
class CProgressDlg;
}

class CProgressDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CProgressDlg(QWidget *parent = nullptr);
    ~CProgressDlg();

private:
    Ui::CProgressDlg *ui;
};

#endif // PROGRESSDLG_H
