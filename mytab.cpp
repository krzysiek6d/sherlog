#include "mytab.h"
#include "ui_mytab.h"

MyTab::MyTab(QWidget *parent, const std::vector<QString>& fileContents) :
    QWidget(parent),
    ui(new Ui::MyTab)
{
    ui->setupUi(this);

    editor = new CodeEditor(this);
    editor->setObjectName(QStringLiteral("textBrowser"));
    ui->horizontalLayout->addWidget(editor);

    for(const auto& line: fileContents)
    {
        //ui->textBrowser1->append(line);
        editor->appendPlainText(line);
    }
}

MyTab::~MyTab()
{
    delete ui;
}
