#include "tabcontainer.h"
#include "ui_tabcontainer.h"
#include "mytab.h"
#include <config.h>
#include <documenttab.h>


TabContainer::TabContainer(QWidget *parent, DocumentTab* tabWithFilename, const FileView& fileContents, MyTab* filterSource) :
    QWidget(parent),
    ui(new Ui::TabContainer),
    tabWithFilename{tabWithFilename}
{
    ui->setupUi(this);
    setFont(Config::getNormalFont());

    std::cout << "TabContainer ctor, FileView size: " << fileContents.size() << std::endl;

    ui->tabWidget->setFont(Config::getNormalFont());
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 22px; }");

    auto baseTab = new MyTab(this, tabWithFilename, fileContents, filterSource);  // deletion is done by tabcloserequest
    ui->tabWidget->addTab(baseTab, "Base");
    ui->tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
}

TabContainer::~TabContainer()
{
    delete ui;
}

void TabContainer::addTab(const FileView& fileContents, const QString& tabName, MyTab* filterSource)
{
    auto filteredTab = new TabContainer(this, tabWithFilename, fileContents, filterSource);
    ui->tabWidget->addTab(filteredTab, tabName); // deletion is done by tabcloserequest
}

void TabContainer::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->widget(index)->deleteLater();
    ui->tabWidget->removeTab(index);
}

void TabContainer::setActiveTab(MyTab* tab)
{
    ui->tabWidget->setCurrentWidget(tab);
}
