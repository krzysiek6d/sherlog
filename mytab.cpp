#include "mytab.h"
#include "ui_mytab.h"
#include "filecontents.h"
#include <QTextCursor>
#include <QTextBlock>

MyTab::MyTab(QWidget *parent, const FileContents& fileContents) :
    QWidget(parent),
    ui(new Ui::MyTab)
{
    ui->setupUi(this);

    editor = new CodeEditor(this);
    editor->setReadOnly(true);
    editor->setTextInteractionFlags(editor->textInteractionFlags() | Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    editor->setObjectName(QStringLiteral("textBrowser"));
    ui->horizontalLayout->addWidget(editor);
    ui->lineEdit->setText(fileContents.filename);

    for(const auto& line: fileContents.data)
    {
        //ui->textBrowser1->append(line);
        editor->appendPlainText(line);
    }
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
