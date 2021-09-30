#include "mytab.h"
#include "ui_mytab.h"
#include "filecontents.h"
#include <QTextCursor>
#include <QTextBlock>
#include <tabcontainer.h>
#include <config.h>
#include <QShortcut>
#include <tabwithfilename.h>

MyTab::MyTab(TabContainer *parent, TabWithFilename* tabWithFilename, const FileView& fileContents) :
    QWidget(parent),
    parent(parent),
    fileContents_{fileContents},
    tabWithFilename{tabWithFilename},
    ui(new Ui::MyTab)
{
    ui->setupUi(this);
    QShortcut *shortcutGrep = new QShortcut(Config::grepShorcut(), this); // rememver to delete
    QObject::connect(shortcutGrep, &QShortcut::activated, [this](){this->focusGrep();});

    QShortcut *shortcutFind = new QShortcut(Config::findShorcut(), this); // rememver to delete
    QObject::connect(shortcutFind, &QShortcut::activated, [this](){this->focusFind();});

    QShortcut *shortcutGoToLine = new QShortcut(Config::gotoLineShorcut(), this); // rememver to delete
    QObject::connect(shortcutGoToLine, &QShortcut::activated, [this](){this->focusGotoLine();});

    QShortcut *shortcutBookmark = new QShortcut(Config::bookmarkShorcut(), this); // rememver to delete
    QObject::connect(shortcutBookmark, &QShortcut::activated, [this](){this->bookmark();});

    setFont(Config::getNormalFont());
    ui->groupBox_2->setFont(Config::getNormalFont());
    ui->gotoLabel->setFont(Config::getNormalFont());
    ui->greplabel->setFont(Config::getNormalFont());
    ui->findLabel->setFont(Config::getNormalFont());
    ui->grepInput->setFont(Config::getNormalFont());
    ui->findInput->setFont(Config::getNormalFont());
    ui->gotoLineInput->setFont(Config::getNormalFont());

    editor = new CodeEditor(this, fileContents_);

    ui->horizontalLayout->addWidget(editor);
}

void MyTab::showEvent( QShowEvent* event ) {
    QWidget::showEvent( event );
    tabWithFilename->setCurrentTab(this);
}

void MyTab::paintEvent( QPaintEvent* event ) {
    QWidget::paintEvent( event );
    tabWithFilename->setCurrentTab(this);
}


MyTab::~MyTab()
{
    std::cout << "deleting mytab" << std::endl;
    tabWithFilename->setCurrentTab(nullptr);
    delete ui;
}

void MyTab::focusGotoLine()
{
    ui->gotoLineInput->setFocus();
}

void MyTab::focusGrep()
{
    ui->grepInput->setFocus();
}

void MyTab::focusFind()
{
    ui->findInput->setFocus();
}

void MyTab::on_findInput_returnPressed()
{
    QTextCursor cursor = this->editor->textCursor();
    QString text = this->ui->findInput->text();
    // if regex, if match case
    QRegExp regex = QRegExp(text);
    this->editor->find(regex);
}

void MyTab::on_grepInput_returnPressed()
{
    const auto& textToSearch = ui->grepInput->text();
    FileView view = fileContents_;
    view.filter(textToSearch);
    parent->addTab(view, {}, textToSearch);
}

void MyTab::on_gotoLineInput_returnPressed()
{
    std::cout << "going to line (text): " << ui->gotoLineInput->text().toStdString() << std::endl;
    int ln = ui->gotoLineInput->text().toUInt();
    if (ln == 0 && ui->gotoLineInput->text() != "0")
        return;
    std::cout << "going to line: " << ln << std::endl;
    QTextCursor cursor(editor->document()->findBlockByLineNumber(ln-1)); // ln-1 because line number starts from 0
    editor->setTextCursor(cursor);
}

void MyTab::bookmark()
{
    std::cout << "bookmarking line: " << editor->getCurrentLineNumber() << std::endl;
    FileView view = fileContents_;
    auto realnum = view[editor->getCurrentLineNumber()]->lineNum;
    std::cout << "real line number: " << realnum << std::endl;
    tabWithFilename->addBookmark(realnum, editor->getSelectedText());
}

void MyTab::gotoLineInFile(int lineNum)
{
    FileView view = fileContents_;
    auto lineIt = std::lower_bound(view.begin(), view.end(), lineNum,
                                   [](const auto& elem, const auto& elem2){return elem->lineNum < elem2;});
    auto localLineNum = std::distance(view.begin(), lineIt);
    std::cout << "want to go to line number in file : " << lineNum << ", this means: goto line in view: " << localLineNum << std::endl;
    QTextCursor cursor(editor->document()->findBlockByLineNumber(localLineNum)); // ln-1 because line number starts from 0
    editor->setTextCursor(cursor);
}
