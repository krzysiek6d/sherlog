#include "tabcontainer.h"
#include "ui_tabcontainer.h"
#include "mytab.h"
#include <config.h>
#include <documenttab.h>
#include <iostream>


TabContainer::TabContainer(QWidget *parent, DocumentTab* documentTab, const FileView& fileContents, MyTab* filterSource) :
    QWidget(parent),
    ui(new Ui::TabContainer),
    documentTab{documentTab}
{
    ui->setupUi(this);
    setFont(Config::getNormalFont());
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, [this](int index){this->onTabCloseRequest(index);});

    std::cout << "TabContainer ctor, FileView size: " << fileContents.size() << std::endl;

    ui->tabWidget->setFont(Config::getNormalFont());
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 22px; }");

    auto baseTab = new MyTab(this, documentTab, fileContents, filterSource);  // deletion is done by tabcloserequest
    ui->tabWidget->addTab(baseTab, "Base");
    ui->tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
}

TabContainer::~TabContainer()
{
}

void TabContainer::addTab(const FileView& fileContents, const QString& tabName, MyTab* filterSource)
{
    auto filteredTab = new TabContainer(this, documentTab, fileContents, filterSource);
    ui->tabWidget->addTab(filteredTab, tabName); // deletion is done by tabcloserequest
}

void TabContainer::onTabCloseRequest(int index)
{
    ui->tabWidget->widget(index)->deleteLater();
    ui->tabWidget->removeTab(index);
}

void TabContainer::switchTab(MyTab* tab)
{
    ui->tabWidget->setCurrentWidget(tab);
}
