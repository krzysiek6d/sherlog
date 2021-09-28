#include "tabcontainer.h"
#include "ui_tabcontainer.h"
#include "mytab.h"
#include <config.h>
#include <tabwithfilename.h>


TabContainer::TabContainer(QWidget *parent, TabWithFilename* tabWithFilename, const FileView& fileContents, const QString& tabName) :
    QWidget(parent),
    ui(new Ui::TabContainer),
    tabWithFilename{tabWithFilename}
{
    ui->setupUi(this);
    setFont(Config::getNormalFont());

    std::cout << "TabContainer ctor, FileView size: " << fileContents.getNumOfLines() << std::endl;

    ui->tabWidget->setFont(Config::getNormalFont());
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 22px; }");
    ui->tabWidget->addTab(new MyTab(this, tabWithFilename, fileContents), "Base");

}

TabContainer::~TabContainer()
{
    delete ui;
}

void TabContainer::addTab(const FileView& fileContents, const std::vector<int>& lines, const QString& tabName)
{
    ui->tabWidget->addTab(new TabContainer(this, tabWithFilename, fileContents, "Base"), tabName);
}

void TabContainer::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
