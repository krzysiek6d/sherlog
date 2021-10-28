#pragma once

#include <QWidget>
#include <fileview.h>
#include <memory>

namespace Ui {
class TabContainer;

}
class DocumentTab;
class MyTab;
class Highlighter;

class TabContainer : public QWidget
{
    Q_OBJECT

public:
    TabContainer(QWidget *parent, DocumentTab* documentTab, const FileView& fileContents, MyTab* filterSource, std::shared_ptr<Highlighter> highlighter);
    void addTab(const FileView& fileContents, const QString& tabName, MyTab* filterSource);
    void switchTab(MyTab* tab);
    ~TabContainer();

private:
    void onTabCloseRequest(int index);

    std::unique_ptr<Ui::TabContainer> ui;
    DocumentTab* documentTab;
    std::shared_ptr<Highlighter> highlighter;
};
