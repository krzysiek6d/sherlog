#ifndef TABCONTAINER_H
#define TABCONTAINER_H

#include <QWidget>
#include <filecontents.h>

namespace Ui {
class TabContainer;

}
class TabWithFilename;
class MyTab;

class TabContainer : public QWidget
{
    Q_OBJECT

public:
    explicit TabContainer(QWidget *parent, TabWithFilename* tabWithFilename, const FileView& fileContents, MyTab* filterSource);
    void addTab(const FileView& fileContents, const std::vector<int>& lines, const QString& tabName, MyTab* filterSource);
    void setActiveTab(MyTab* tab);
    ~TabContainer();

private slots:
    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::TabContainer *ui;
    TabWithFilename* tabWithFilename;
};

#endif // TABCONTAINER_H
