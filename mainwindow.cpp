#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <iostream>
#include <mytab.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(2);
//    logArea = std::make_unique<CodeEditor>(ui->horizontalLayoutWidget);
//    logArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    logArea->adjustSize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    auto filenames = QFileDialog::getOpenFileNames(this);
    std::cout << "open clicked" << std::endl;
    for (auto&& filename: filenames)
    {
        std::cout << "opening " << filename.toStdString() << std::endl;
        QFile file{filename};
        if (file.open(QFileDevice::ReadOnly | QFileDevice::Text))
        {
            FileContents fileContents{};
            fileContents.filename = filename;
            auto tabName = fileContents.getShortFilename();
            std::cout << "reading " << filename.toStdString() << std::endl;
            QTextStream in(&file);
            while (!in.atEnd())
            {
               fileContents.data.emplace_back(file.readLine());
            }
            filesContents.emplace_back(std::move(fileContents));
            MyTab *tab = new MyTab(ui->tabWidget, filesContents.back());
            ui->tabWidget->addTab(tab, tabName);
            file.close();
        }
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
