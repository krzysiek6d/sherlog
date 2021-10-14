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

MyTab::MyTab(TabContainer *parent, DocumentTab* tabWithFilename, const FileView& fileContents, MyTab* filterSource) :
    QWidget(parent),
    parent(parent),
    fileContents_{fileContents},
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

    editor = new LogArea(this, fileContents_);

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
    QTextCursor cursor = this->editor->textCursor();
    QString text = this->ui->findInput->text();

    auto matchCase = this->ui->findMatchCase->isChecked();
    QTextDocument::FindFlags options;
    if (matchCase)
        options |= QTextDocument::FindCaseSensitively;
    if (findPrev)
        options |= QTextDocument::FindBackward;

    Qt::CaseSensitivity cs = matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;


    auto smartFind = [this, cs, options](const QString& pattern){
        if (this->ui->findRegex->isChecked())
        {
            QRegExp regex = QRegExp(pattern, cs);
            return this->editor->find(regex, options);
        }
        else
        {
            return this->editor->find(pattern, options);
        }
    };

    if (not smartFind(text))
    {
        QTextCursor cursorBackup(editor->textCursor());
        const int verticalScrollBarBackup = editor->verticalScrollBar()->value();
        const int horizontalScrollBarBackup = editor->horizontalScrollBar()->value();

        QMessageBox* loopMsgBox = new QMessageBox(QMessageBox::Information,
                                                  QString("Search"),
                                                  QString("No more results found, looping the file"),
                                                  QMessageBox::Ok,
                                                  this);
        loopMsgBox->show();
        if (!findPrev)
        {
            QTextCursor cursor(editor->document()->firstBlock());
            editor->setTextCursor(cursor);
        }
        else
        {
            QTextCursor cursor(editor->document()->lastBlock());
            editor->setTextCursor(cursor);
        }
        if (not smartFind(text))
        {
            editor->setTextCursor(cursorBackup);
            editor->verticalScrollBar()->setValue(verticalScrollBarBackup);
            editor->horizontalScrollBar()->setValue(horizontalScrollBarBackup);
        }
    }
}

void MyTab::on_grepInput_returnPressed()
{
    const auto& textToSearch = ui->grepInput->text();
    const auto& newTabName = textToSearch;
    FileView view = fileContents_;
    auto matchCase = this->ui->grepMatchCase->isChecked();
    auto reverse = this->ui->grepReverse->isChecked();
    auto regex = this->ui->grepRegex->isChecked();
    QString optionsStr = QString(" [") + (matchCase ? "C" : "c") + (reverse ? "R" : "r") + (regex ? "X" : "x") + "]";
    view.filter(textToSearch, matchCase, reverse, regex);
    parent->addTab(view, newTabName + optionsStr, this);
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
    if (editor->getCurrentLineNumber() < view.size())
    {
        auto realnum = view[editor->getCurrentLineNumber()]->lineNum;
        std::cout << "real line number: " << realnum << std::endl;
        tabWithFilename->addBookmark(realnum, editor->getSelectedText());
    }
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

void MyTab::showLineInFilterSource(int realnum)
{
    if (filterSource_)
    {
        filterSource_->parent->setActiveTab(filterSource_);
        filterSource_->gotoLineInFile(realnum);
    }
}
