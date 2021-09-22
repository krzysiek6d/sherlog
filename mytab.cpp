#include "mytab.h"
#include "ui_mytab.h"
#include "filecontents.h"
#include <QTextCursor>
#include <QTextBlock>
#include <tabcontainer.h>
#include <config.h>


MyTab::MyTab(TabContainer *parent, const FileView& fileContents) :
    QWidget(parent),
    parent(parent),
    fileContents_{fileContents},
    ui(new Ui::MyTab)
{
    ui->setupUi(this);

    setFont(Config::getNormalFont());
    ui->groupBox->setFont(Config::getNormalFont());
    ui->groupBox_2->setFont(Config::getNormalFont());
    ui->label->setFont(Config::getNormalFont());
    ui->lineEdit->setFont(Config::getNormalFont());
    ui->lineEdit_2->setFont(Config::getNormalFont());

    editor = new CodeEditor(this, fileContents_);

    ui->horizontalLayout->addWidget(editor);
}

MyTab::~MyTab()
{
    delete ui;
}

void MyTab::on_lineEdit_2_returnPressed()
{
    std::cout << "going to line (text): " << ui->lineEdit_2->text().toStdString() << std::endl;
    int ln = ui->lineEdit_2->text().toUInt();
    if (ln == 0 && ui->lineEdit_2->text() != "0")
        return;
    std::cout << "going to line: " << ln << std::endl;
    QTextCursor cursor(editor->document()->findBlockByLineNumber(ln-1)); // ln-1 because line number starts from 0
    editor->setTextCursor(cursor);
}

void MyTab::on_lineEdit_returnPressed()
{
    const auto& textToSearch = ui->lineEdit->text();
    FileView view = fileContents_;
    view.filter(textToSearch);
    parent->addTab(view, {}, textToSearch);
}
