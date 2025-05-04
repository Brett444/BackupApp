#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include <sstream>
#include <string>
#include <filesystem>

#include "MyCString.h"

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QSettings settings("BrettPrograms", "BackupApp");

    BackupStr = settings.value("BackupFolder", "C:").toString();
    DestStr = settings.value("DestFolder", "D:").toString();

    ui->setupUi(this);

    ui->BackupFolder_lbl->setText(BackupStr);
    ui->DestFolder_lbl->setText(DestStr);
}

MainWindow::~MainWindow()
{
    QSettings settings("BrettPrograms", "BackupApp");

    settings.setValue("BackupFolder", BackupStr.toStdString().c_str());
    settings.setValue("DestFolder", DestStr.toStdString().c_str());

    delete ui;
}

void MainWindow::on_BackupFolder_btn_clicked()
{
    QString buf = QFileDialog::getExistingDirectory(this, "Select Folder To Backup", BackupStr.toStdString().c_str(), QFileDialog::ShowDirsOnly);

    if (!buf.isEmpty())
    {
        BackupStr = buf;
        ui->BackupFolder_lbl->setText(BackupStr);
    }
}

void MainWindow::on_DestFolder_btn_clicked()
{
    QString buf = QFileDialog::getExistingDirectory(this, "Select Destination Folder", DestStr.toStdString().c_str(), QFileDialog::ShowDirsOnly);

    if (!buf.isEmpty())
    {
        DestStr = buf;
        ui->DestFolder_lbl->setText(DestStr);
    }
}

void MainWindow::on_StartBackup_btn_clicked()
{
    std::stringstream time_ss;
    CMyCString backuppath, destpath, endfolder, timestr;
    bool createdirok;

    backuppath = BackupStr.toStdString();
    destpath = DestStr.toStdString();
    if (backuppath.Contains("/"))
        endfolder = backuppath.GetSubStr2(-1, '/');  // if path is "C:/", endfolder will be empty

    if (endfolder.IsEmpty())
        endfolder = backuppath.GetSubStr1(0, 0);

    std::time_t now = std::time(nullptr);
    std::tm *p_localtime = std::localtime(&now);

    time_ss << std::put_time(p_localtime, "%Y_%m_%d %H_%M_%S");
    timestr = time_ss.str();

    destpath.AppendStr("/");
    destpath.AppendStr(endfolder.c_str());
    destpath.AppendStr(" ");
    destpath.AppendStr(timestr.c_str());

    fs::path src_obj = backuppath.c_str();
    fs::path dest_obj = destpath.c_str();

    try
    {
        createdirok = fs::create_directory(dest_obj);
    }
    catch (std::exception& e)
    {
        QMessageBox::information(this, "Error Creating Folder", "Could Not Create Destination Folder", QMessageBox::Ok);
        return;
    }

    if (!createdirok)
    {
        QMessageBox::information(this, "Error Creating Folder", "Could Not Create Destination Folder", QMessageBox::Ok);
        return;
    }

    try
    {
        fs::copy(src_obj, dest_obj, fs::copy_options::overwrite_existing | fs::copy_options::recursive);
    }
    catch (std::exception& e)
    {
        QMessageBox::information(this, "Error Copying Files", e.what(), QMessageBox::Ok);
        return;
    }

    QMessageBox::information(this, "Backup Status", "Backup Complete.", QMessageBox::Ok);
}

