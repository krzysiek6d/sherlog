#include "tabwithfilename.h"
#include "ui_tabwithfilename.h"
#include "tabcontainer.h"
#include <mytab.h>
#include <config.h>
#include <iostream>
#include <QShortcut>


TabWithFilename::TabWithFilename(QWidget *parent, const FileContents& fileContents) :
    QWidget(parent),
    ui(new Ui::tabWithFilename),
    fileView(fileContents)
{
    ui->setupUi(this);
    ui->lineEdit->setText(fileContents.filename);
    auto container = new TabContainer(this, this, fileView, "Base");
    ui->horizontalLayout->addWidget(container);

    connect(ui->bookmarksList, &QListWidget::doubleClicked, [this, container](const QModelIndex &item){
        std::cout << "item " << item.row() << " clicked!" << std::endl;
    });
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->bookmarksList);
    connect(shortcut, &QShortcut::activated, [this](){
        deleteBookmark(ui->bookmarksList->currentIndex().row());
    });
}

TabWithFilename::~TabWithFilename()
{
    ui->bookmarksList->clear();
    std::cout << __func__ << std::endl;
}

void TabWithFilename::addBookmark(int num, const QString& text)
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


void TabWithFilename::deleteBookmark(int index)
{
    std::cout << "delete bookmark" << std::endl;
    ui->bookmarksList->clear();
    bookmarks.erase(bookmarks.begin() + index);
    for (const auto& bookmark: bookmarks)
    {
        auto* item = new QListWidgetItem(bookmark.text);
        ui->bookmarksList->addItem(item);
    }
}
