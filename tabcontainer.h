#ifndef TABCONTAINER_H
#define TABCONTAINER_H

#include <QWidget>
#include <filecontents.h>
#include <memory>

namespace Ui {
class TabContainer;

}
class DocumentTab;
class MyTab;

class TabContainer : public QWidget
{
    Q_OBJECT

public:
    TabContainer(QWidget *parent, DocumentTab* documentTab, const FileView& fileContents, MyTab* filterSource);
    void addTab(const FileView& fileContents, const QString& tabName, MyTab* filterSource);
    void switchTab(MyTab* tab);
    ~TabContainer();

private:
    void onTabCloseRequest(int index);

    std::unique_ptr<Ui::TabContainer> ui;
    DocumentTab* documentTab;
};

#endif // TABCONTAINER_H
