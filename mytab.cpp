#include "mytab.h"
#include "ui_mytab.h"

MyTab::MyTab(QWidget *parent, const std::vector<QString>& fileContents) :
    QWidget(parent),
    ui(new Ui::MyTab)
{
    ui->setupUi(this);
    for(const auto& line: fileContents)
    {
        ui->textBrowser->append(line);
    }
}

MyTab::~MyTab()
{
    delete ui;
}
