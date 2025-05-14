#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

#include <QDir>
#include <QDirIterator>
#include <QFile>

#include <iostream>
#include <fstream>

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
    LogFails = true;

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

void MainWindow::LogFileFail(const QString &filepath)
{
    std::ofstream logfile;
    QString timestr, logstr;
    CMyCString logpath;
    char charbuf[50];
    std::tm timeinfo;

    logpath = DestFullPath + "/LogFile_BackupApp.txt";

    std::time_t now = std::time(nullptr);
    localtime_s(&timeinfo, &now);
    std::strftime(charbuf, 50, "%Y-%m-%d %H:%M:%S", &timeinfo);
    timestr = charbuf;

    logstr = timestr + QString(", ") + filepath;

    logfile.open(logpath, std::ios::app);

    if (logfile.is_open())
    {
        logfile << logstr.toStdString() << std::endl;
        logfile.close();
    }
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
    CMyCString backuppath, endfolder, timestr;
    bool createdirok, backupok;
    std::tm timeinfo;
    char charbuf[50];  // 50 hardcoded here and below.

    backuppath = BackupStr.toStdString();
    DestFullPath = DestStr.toStdString();
    if (backuppath.Contains("/"))
        endfolder = backuppath.GetSubStr2(-1, '/');  // if path is "C:/", endfolder will be empty

    if (endfolder.IsEmpty())
        endfolder = backuppath.GetSubStr1(0, 0);

    std::time_t now = std::time(nullptr);
    localtime_s(&timeinfo, &now);
    std::strftime(charbuf, 50, "%Y_%m_%d %H_%M_%S", &timeinfo);
    timestr = charbuf;

    DestFullPath.AppendStr("/");
    DestFullPath.AppendStr(endfolder.c_str());
    DestFullPath.AppendStr(" ");
    DestFullPath.AppendStr(timestr.c_str());

    fs::path src_obj = backuppath.c_str();
    fs::path dest_obj = DestFullPath.c_str();

    try
    {
        createdirok = fs::create_directory(dest_obj);
    }
    catch (std::exception& e)
    {
        ui->StatusBar->showMessage("Error Creating Destination Folder");
        QMessageBox::information(this, "Error Creating Destination Folder", e.what(), QMessageBox::Ok);
        return;
    }

    if (!createdirok)
    {
        ui->StatusBar->showMessage("Could Not Create Destination Folder");
        QMessageBox::information(this, "Error Creating Folder", "Could Not Create Destination Folder", QMessageBox::Ok);
        return;
    }

    try
    {
        // this is the c++ way to do it but is just blocks.
        //fs::copy(src_obj, dest_obj, fs::copy_options::overwrite_existing | fs::copy_options::recursive);

        NumFiles = NumFilesInFolder(backuppath.c_str());
        p_progressdlg = new CProgressDlg();
        p_progressdlg->SetFileCount(NumCopied, NumFiles);
        p_progressdlg->show();
        backupok = CopyDirectory(backuppath.c_str(), DestFullPath.c_str());
    }
    catch (std::exception& e)
    {
        QMessageBox::information(this, "Error Copying Files", e.what(), QMessageBox::Ok);
        delete p_progressdlg;
        ui->StatusBar->showMessage("Backup Failed.");
        return;
    }

    if (p_progressdlg->BackupWasCanceled)
        ui->StatusBar->showMessage("Backup Canceled.");
    else if (!backupok)
        ui->StatusBar->showMessage("Backup Failed.");
    else
        ui->StatusBar->showMessage("Backup Completed.");

    delete p_progressdlg;
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

    ui->StatusBar->showMessage("Backup In Progress...");
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
                if (LogFails)
                    LogFileFail(filePath);
                CMyCString errstr;
                errstr.Format("Failed to copy %s to %s.", filePath.toStdString().c_str(), destFilePath.toStdString().c_str());
                QMessageBox::information(this, "File Copy Failed, Click Ok To Continue...", "errstr.c_str()", QMessageBox::Ok);
                //return false;
            }
            else
            {
                // LogFileFail(filePath);  // for testing
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // for testing
                QCoreApplication::processEvents();
                NumCopied += 1;
                if (p_progressdlg->BackupWasCanceled)
                    return false;
                p_progressdlg->SetFileCount(NumCopied, NumFiles);
            }
        }
    }
    return true;
}

void MainWindow::on_LogFiles_chk_checkStateChanged(const Qt::CheckState &arg1)
{
    LogFails = !LogFails;
}

