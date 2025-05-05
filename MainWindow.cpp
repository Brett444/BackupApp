#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include <QDir>
#include <QDirIterator>
#include <QFile>

#include <sstream>
#include <string>
#include <filesystem>
#include <thread>
#include <chrono>

#include "MyCString.h"

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    QSettings settings("BrettPrograms", "BackupApp");

    BackupStr = settings.value("BackupFolder", "C:").toString();
    DestStr = settings.value("DestFolder", "D:").toString();

    NumCopied = 0;

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
        QMessageBox::information(this, "Error Creating Destinatin Folder", e.what(), QMessageBox::Ok);
        return;
    }

    if (!createdirok)
    {
        QMessageBox::information(this, "Error Creating Folder", "Could Not Create Destination Folder", QMessageBox::Ok);
        return;
    }

    try
    {
        //fs::copy(src_obj, dest_obj, fs::copy_options::overwrite_existing | fs::copy_options::recursive);

        NumFiles = NumFilesInFolder(backuppath.c_str());
        p_progressdlg = new QProgressDialog("Backup in progress.", "Cancel", 0, NumFiles);
        p_progressdlg->setWindowModality(Qt::ApplicationModal);
        p_progressdlg->show();
        p_progressdlg->setValue(0);
        CopyDirectory(backuppath.c_str(), destpath.c_str());
        delete p_progressdlg;
    }
    catch (std::exception& e)
    {
        delete p_progressdlg;
        QMessageBox::information(this, "Error Copying Files", e.what(), QMessageBox::Ok);
        return;
    }

    QMessageBox::information(this, "Backup Status", "Backup Complete.", QMessageBox::Ok);
}

uint32 MainWindow::NumFilesInFolder(const QString& dirPath)
{
    QDirIterator it(dirPath, QDir::Files, QDirIterator::Subdirectories);
    uint32 count = 0;

    while (it.hasNext())
    {
        it.next();
        count++;
    }
    return count;
}

// copies all files including subfolders
bool MainWindow::CopyDirectory(const QString &sourceDir, const QString &destinationDir)
{
    QDir dir(sourceDir);
    if (!dir.exists())
        return false;

    QDir destDir(destinationDir);
    if (!destDir.exists())
    {
        if (!destDir.mkpath("."))
            return false;
    }

    NumCopied = 0;
    QDirIterator it(sourceDir, QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        QString filePath = it.filePath();
        QString relativePath = it.fileInfo().isDir() ? it.fileName() : it.filePath().mid(sourceDir.length() + 1);
        QString destFilePath = destinationDir + "/" + relativePath;
        if (it.fileInfo().isDir())
        {
            QDir subDir(destFilePath);
            if (!subDir.exists())
            {
                if(!subDir.mkpath("."))
                    return false;
            }
        }
        else
        {
            if (!QFile::copy(filePath, destFilePath))
            {
                CMyCString errstr;
                errstr.Format("Failed to copy %s to %s.", filePath.toStdString().c_str(), destFilePath.toStdString().c_str());
                QMessageBox::information(this, "Copy Failed", "errstr.c_str()", QMessageBox::Ok);
                return false;
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
            NumCopied += 1;
            if (p_progressdlg->wasCanceled())
                return false;
            p_progressdlg->setValue(NumCopied);
        }
    }
    return true;
}

