#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <iostream>
#include <mytab.h>
#include <tabwithfilename.h>
#include <config.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
            int lineNum = 0;
            while (!in.atEnd())
            {
                QString line = file.readLine();

                Line l {lineNum, line.trimmed()};
               fileContents.data.emplace_back(l);
               lineNum++;
            }
            filesContents.emplace_back(std::move(fileContents));
            tabWithFilename *tab = new tabWithFilename(ui->tabWidget, filesContents.back());
            ui->tabWidget->addTab(tab, tabName);
            file.close();
        }
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
