#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "PndDefs.h"
#include "MyCString.h"

#include "ProgressDlg.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void LogFileFail(const QString &filepath);

private:
    QString BackupStr, DestStr;
    CMyCString DestFullPath;
    uint32 NumCopied, NumFiles;
    bool LogFails;
    CProgressDlg *p_progressdlg;

    uint32 NumFilesInFolder(const QString& dirPath);
    bool CopyDirectory(const QString &sourceDir, const QString &destinationDir);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BackupFolder_btn_clicked();
    void on_DestFolder_btn_clicked();
    void on_StartBackup_btn_clicked();

    void on_LogFiles_chk_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
