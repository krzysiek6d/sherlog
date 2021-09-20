#include "tabcontainer.h"
#include "ui_tabcontainer.h"
#include "mytab.h"
#include <config.h>


TabContainer::TabContainer(QWidget *parent, const FileContents& fileContents, const QString& tabName) :
    QWidget(parent),
    ui(new Ui::TabContainer)
{
    ui->setupUi(this);
    setFont(Config::getNormalFont());

    ui->tabWidget->setFont(Config::getNormalFont());
    ui->tabWidget->setStyleSheet("QTabBar::tab { height: 22px; }");
    ui->tabWidget->addTab(new MyTab(this, fileContents), "Base");
}

TabContainer::~TabContainer()
{
    delete ui;
}

void TabContainer::addTab(const FileContents& fileContents, const std::vector<int>& lines, const QString& tabName)
{
    ui->tabWidget->addTab(new TabContainer(this, fileContents, "Base"), tabName);
}

void TabContainer::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
