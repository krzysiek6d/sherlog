#ifndef MYTAB_H
#define MYTAB_H

#include <QWidget>
#include <logarea.h>
#include <memory>
#include <filecontents.h>

namespace Ui {
class MyTab;
}

class MyTab : public QWidget
{
    Q_OBJECT

public:
    //explicit MyTab(QWidget *parent = 0, const std::vector<QString>& fileContents, const std::vector<int>& linesToShow);
    explicit MyTab(QWidget *parent, const FileContents& fileContents);
    ~MyTab();

private slots:
    void on_lineEdit_2_returnPressed();

private:
    CodeEditor* editor;
    Ui::MyTab *ui;
};

#endif // MYTAB_H
