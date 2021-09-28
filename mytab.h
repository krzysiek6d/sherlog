#ifndef MYTAB_H
#define MYTAB_H

#include <QWidget>
#include <logarea.h>
#include <memory>
#include <filecontents.h>

namespace Ui {
class MyTab;
}

class TabContainer;
class TabWithFilename;

class MyTab : public QWidget
{
    Q_OBJECT

public:
    MyTab(TabContainer *parent, TabWithFilename* tabWithFilename, const FileView& fileContents);
    ~MyTab();

private slots:
    void on_findInput_returnPressed();
    void on_grepInput_returnPressed();
    void on_gotoLineInput_returnPressed();

private:
    void focusGotoLine();
    void focusFind();
    void focusGrep();
    void bookmark();
    TabContainer *parent;
    FileView fileContents_;
    CodeEditor* editor;
    TabWithFilename* tabWithFilename;
    Ui::MyTab *ui;
};

#endif // MYTAB_H
