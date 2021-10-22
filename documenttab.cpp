#include "documenttab.h"
#include "ui_documenttab.h"
#include "tabcontainer.h"
#include <mytab.h>
#include <config.h>
#include <iostream>
#include <QShortcut>
#include <filecontents.h>


DocumentTab::DocumentTab(QWidget *parent, const FileContents& fileContents) :
    QWidget(parent),
    ui(new Ui::documentTab),
    fileView(fileContents),
    currentTab(nullptr)
{
    ui->setupUi(this);
    ui->lineEdit->setText(fileContents.filename());
    auto container = new TabContainer(this, this, fileView, nullptr);
    ui->splitter->addWidget(container);
    ui->splitter->setSizes(QList<int>() << 1 << 1); // dont know how it works but it sets size properly ;D

    connect(ui->bookmarksList, &QListWidget::doubleClicked, [this](const QModelIndex &item){
        std::cout << "item " << item.row() << " clicked!" << std::endl;
        if (currentTab)
        {
            currentTab->gotoLineInFile(bookmarks[item.row()].linenum);
        }
    });
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->bookmarksList);
    connect(shortcut, &QShortcut::activated, [this](){
        deleteBookmark(ui->bookmarksList->currentIndex().row());
    });
}

DocumentTab::~DocumentTab()
{
    ui->bookmarksList->clear();
    std::cout << __func__ << std::endl;
}

void DocumentTab::addBookmark(int num, const QString& text)
{
    std::cout << "num of bookmarks is: " << bookmarks.size() << std::endl;

    Bookmark newBookmark;
    newBookmark.linenum = num;
    newBookmark.text = QString::number(num) + ": " + text;
    bookmarks.emplace_back(std::move(newBookmark));

    ui->bookmarksList->clear();
    for (const auto& bookmark: bookmarks)
    {
        auto* item = new QListWidgetItem(bookmark.text);
        ui->bookmarksList->addItem(item);
    }
    std::cout << "num of bookmarks is: " << bookmarks.size() << std::endl;

}


void DocumentTab::deleteBookmark(int index)
{
    if (index < 0)
        return;
    std::cout << "delete bookmark at" << index << std::endl;
    ui->bookmarksList->clear();
    bookmarks.erase(bookmarks.begin() + index);
    for (const auto& bookmark: bookmarks)
    {
        auto* item = new QListWidgetItem(bookmark.text);
        ui->bookmarksList->addItem(item);
    }
}

void DocumentTab::setCurrentTab(MyTab* tab)
{
    currentTab = tab;
}
