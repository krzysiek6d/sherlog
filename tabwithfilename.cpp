#include "tabwithfilename.h"
#include "ui_tabwithfilename.h"
#include "tabcontainer.h"
#include <mytab.h>
#include <config.h>


tabWithFilename::tabWithFilename(QWidget *parent, const FileContents& fileContents) :
    QWidget(parent),
    ui(new Ui::tabWithFilename)
{
    ui->setupUi(this);
    ui->lineEdit->setText(fileContents.filename);
    ui->verticalLayout->addWidget(new TabContainer(this, fileContents, "Base"));
}

tabWithFilename::~tabWithFilename()
{
    delete ui;
}
