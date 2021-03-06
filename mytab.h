#pragma once

#include <QWidget>
#include <logarea.h>
#include <memory>
#include <fileview.h>

namespace Ui {
class MyTab;
}

class TabContainer;
class DocumentTab;
class Highlighter;

class MyTab : public QWidget
{
    using FindBackward = bool;
    Q_OBJECT

public:
    MyTab(TabContainer *parent, DocumentTab* tabWithFilename, FileView fileView, MyTab* filterSource, std::shared_ptr<Highlighter> highlighter);
    ~MyTab();

    void showEvent( QShowEvent* event );
    void paintEvent( QPaintEvent* event ) ;

    void gotoLineInFile(int);
    void showLineInFilterSource(int);

private slots:
    void on_grepInput_returnPressed();
    void on_gotoLineInput_returnPressed();

private:
    void search(FindBackward);
    void focusGotoLine();
    void focusFind();
    void focusGrep();
    void bookmark();
    TabContainer *parent;
    FileView fileView_;
    LogArea* editor;
    DocumentTab* tabWithFilename;
    Ui::MyTab *ui;
    MyTab* filterSource_;
};
