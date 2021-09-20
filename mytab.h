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

class MyTab : public QWidget
{
    Q_OBJECT

public:
    //explicit MyTab(QWidget *parent = 0, const std::vector<QString>& fileContents, const std::vector<int>& linesToShow);
    explicit MyTab(TabContainer *parent, const FileContents& fileContents);
    ~MyTab();

private slots:
    void on_lineEdit_2_returnPressed();

    void on_lineEdit_returnPressed();

private:
    TabContainer *parent;
    const FileContents& fileContents_;
    CodeEditor* editor;
    Ui::MyTab *ui;
};

#endif // MYTAB_H
