#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <iostream>
#include <mytab.h>
#include <documenttab.h>
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
        FileContents fileContents{filename};
        if (fileContents.read())
        {
            auto tabName = fileContents.getShortFilename();
            filesContents.emplace_back(std::move(fileContents));
            DocumentTab *tab = new DocumentTab(ui->tabWidget, filesContents.back());
            ui->tabWidget->addTab(tab, tabName);
        }
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
