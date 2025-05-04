#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QString BackupStr, DestStr;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BackupFolder_btn_clicked();
    void on_DestFolder_btn_clicked();
    void on_StartBackup_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
