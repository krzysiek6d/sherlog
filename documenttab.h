#pragma once

#include <QWidget>
#include <fileview.h>
#include <memory>
#include <QListWidgetItem>
#include <bookmark.h>
#include <highlighter.h>

namespace Ui {
class documentTab;
}
class MyTab;

class DocumentTab : public QWidget
{
    Q_OBJECT

public:
    DocumentTab(QWidget *parent, const FileContents& fileContents);
    ~DocumentTab();

    void addBookmark(int num, const QString& text);
    void deleteBookmark(int index);
    void setCurrentTab(MyTab* tab);

private:
    std::vector<Bookmark> bookmarks;
    std::unique_ptr<Ui::documentTab> ui;
    FileView fileView;
    MyTab* currentTab;
    Highlighter highlighter;
};

