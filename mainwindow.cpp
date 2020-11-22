#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDirIterator>
#include <QFileInfo>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,currentFolder("d:/")
    ,model(new QStringListModel)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}


void MainWindow::on_chooseBtn_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,tr("选择文件夹"),
                                                     currentFolder);
    ui->folderLE->setText(path);
    currentFolder = path;

    QDirIterator it(path, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList files;
    while (it.hasNext())
        files << it.next();
    files.sort();

    model->setStringList(files);
    ui->listView->setModel(model);
}

void MainWindow::on_suffixBtn_clicked()
{
    QString before = ui->suffixBeforeLE->text();
    QString after = ui->suffixAfterLE->text();

    QStringList list;//目标文件集合
    QStringList all = model->stringList();
    for(QString filePath : all){
        QFileInfo file(filePath);
        if(file.suffix() == before){
            list.append(filePath);
        }
    }

    QString beforeSuffix = before.prepend(".");
    QString afterSuffix = after.prepend(".");
    for(QString filePath : list){
        QFile file(filePath);
        QString target = filePath.replace(beforeSuffix,afterSuffix,Qt::CaseInsensitive);
        file.rename(target);
    }

    ui->statusbar->showMessage("完成",10000);
}

void MainWindow::on_resetBtn_clicked()
{
    QDirIterator it(currentFolder, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList files;
    while (it.hasNext())
        files << it.next();
    files.sort();

    model->setStringList(files);
    ui->listView->setModel(model);
}
