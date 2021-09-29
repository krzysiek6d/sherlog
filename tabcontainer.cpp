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

    auto baseTab = new MyTab(this, tabWithFilename, fileContents);  // deletion is done by tabcloserequest
    ui->tabWidget->addTab(baseTab, "Base");
}

TabContainer::~TabContainer()
{
    delete ui;
}

void TabContainer::addTab(const FileView& fileContents, const std::vector<int>& lines, const QString& tabName)
{
    ui->tabWidget->addTab(new TabContainer(this, tabWithFilename, fileContents, "Base"), tabName); // deletion is done by tabcloserequest
}

void TabContainer::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->widget(index)->deleteLater();
    ui->tabWidget->removeTab(index);
}
