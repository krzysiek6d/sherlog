#pragma once
#include <QWidget>
#include <filecontents.h>
#include <memory>
#include <QListWidgetItem>

namespace Ui {
class documentTab;
}
class MyTab;

struct Bookmark
{
    int linenum;
    QString text;
};

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
};

