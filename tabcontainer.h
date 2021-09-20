#ifndef TABCONTAINER_H
#define TABCONTAINER_H

#include <QWidget>
#include <filecontents.h>

namespace Ui {
class TabContainer;
}

class TabContainer : public QWidget
{
    Q_OBJECT

public:
    explicit TabContainer(QWidget *parent, const FileContents& fileContents, const QString& tabName);
    void addTab(const FileContents& fileContents, const std::vector<int>& lines, const QString& tabName);
    ~TabContainer();

private slots:
    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::TabContainer *ui;
};

#endif // TABCONTAINER_H
