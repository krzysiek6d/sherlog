#include "mytab.h"
#include "ui_mytab.h"
#include "filecontents.h"
#include <QTextCursor>
#include <QTextBlock>
#include <tabcontainer.h>
#include <config.h>
#include <QShortcut>
#include <documenttab.h>
#include <QMessageBox>
#include <QScrollBar>

MyTab::MyTab(TabContainer *parent, DocumentTab* tabWithFilename, FileView fileContents, MyTab* filterSource) :
    QWidget(parent),
    parent(parent),
    fileContents_{std::move(fileContents)},
    editor(new LogArea(this, fileContents_)),
    tabWithFilename{tabWithFilename},
    ui(new Ui::MyTab),
    filterSource_{filterSource}
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

    // FIND
    connect(ui->findInput, &QLineEdit::returnPressed, [this]{this->search(FindBackward(false));});
    QShortcut *findNext = new QShortcut(Config::findNextShorcut(), this); // rememver to delete
    QObject::connect(findNext, &QShortcut::activated, [this](){this->search(FindBackward(false));});
    QShortcut *findPrev = new QShortcut(Config::findPrevShorcut(), this); // rememver to delete
    QObject::connect(findPrev, &QShortcut::activated, [this](){this->search(FindBackward(true));});

    // Checkbox should focus the line they are reffering to
    connect(ui->findMatchCase, &QCheckBox::clicked, [this](bool){this->focusFind();});
    connect(ui->findRegex, &QCheckBox::clicked, [this](bool){this->focusFind();});
    connect(ui->grepMatchCase, &QCheckBox::clicked, [this](bool){this->focusGrep();});
    connect(ui->grepRegex, &QCheckBox::clicked, [this](bool){this->focusGrep();});
    connect(ui->grepReverse, &QCheckBox::clicked, [this](bool){this->focusGrep();});


    setFont(Config::getNormalFont());
    ui->groupBox_2->setFont(Config::getNormalFont());
    ui->gotoLabel->setFont(Config::getNormalFont());
    ui->greplabel->setFont(Config::getNormalFont());
    ui->grepRegex->setFont(Config::getNormalFont());
    ui->grepReverse->setFont(Config::getNormalFont());
    ui->findLabel->setFont(Config::getNormalFont());
    ui->grepInput->setFont(Config::getNormalFont());
    ui->grepMatchCase->setFont(Config::getNormalFont());
    ui->findInput->setFont(Config::getNormalFont());
    ui->findMatchCase->setFont(Config::getNormalFont());
    ui->findRegex->setFont(Config::getNormalFont());
    ui->gotoLineInput->setFont(Config::getNormalFont());

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

void MyTab::search(FindBackward findPrev)
{
    QString text = this->ui->findInput->text();
    auto matchCase = this->ui->findMatchCase->isChecked();
    auto backward = findPrev;
    auto regex = this->ui->findRegex->isChecked();
    editor->search(text, backward, matchCase, regex);
}

void MyTab::on_grepInput_returnPressed()
{
    const auto& textToSearch = ui->grepInput->text();
    const auto& newTabName = textToSearch;

    //TODO: create prepareFilterOptionsInfoStr...
    auto matchCase = this->ui->grepMatchCase->isChecked();
    auto reverse = this->ui->grepReverse->isChecked();
    auto regex = this->ui->grepRegex->isChecked();
    QString optionsStr = QString(" [") + (matchCase ? "C" : "c") + (regex ? "X" : "x") + (reverse ? "R" : "r") + "]";

    FileView view = fileContents_;
    view.filter(textToSearch, matchCase, reverse, regex);

    parent->addTab(view, newTabName + optionsStr, this);
}

void MyTab::on_gotoLineInput_returnPressed()
{
    int ln = ui->gotoLineInput->text().toUInt();
    if (ln == 0 && ui->gotoLineInput->text() != "0")
        return;
    editor->gotoBlockNum(ln-1);
}

void MyTab::bookmark()
{
    tabWithFilename->addBookmark(editor->getCurrentLineNumber(), editor->getSelectedText());
}

void MyTab::gotoLineInFile(int lineNum)
{
    editor->gotoLine(lineNum);
}

void MyTab::showLineInFilterSource(int realnum)
{
    if (filterSource_)
    {
        filterSource_->parent->switchTab(filterSource_);
        filterSource_->gotoLineInFile(realnum);
    }
}
