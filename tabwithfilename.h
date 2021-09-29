#ifndef TABWITHFILENAME_H
#define TABWITHFILENAME_H

#include <QWidget>
#include <filecontents.h>
#include <memory>
#include <QListWidgetItem>

namespace Ui {
class tabWithFilename;
}
class MyTab;

struct Bookmark
{
    int linenum;
    QString text;
};

class TabWithFilename : public QWidget
{
    Q_OBJECT

public:
    TabWithFilename(QWidget *parent, const FileContents& fileContents);
    ~TabWithFilename();

    void addBookmark(int num, const QString& text);
    void deleteBookmark(int index);
    void setCurrentTab(MyTab* tab);

private:
    std::vector<Bookmark> bookmarks;
    std::unique_ptr<Ui::tabWithFilename> ui;
    FileView fileView;
    MyTab* currentTab;
};

#endif // TABWITHFILENAME_H
