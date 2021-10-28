#include "logarea.h"

#include <QPainter>
#include <QTextBlock>
#include <config.h>
#include <filecontents.h>
#include <QShortcut>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QMessageBox>
#include <algorithm>
#include <ranges>
#include <cassert>
#include <thread>
#include <future>
#include <QTimer>
#include <QScrollBar>
#include <mytab.h>
#include <timer.h>


LogArea::LogArea(MyTab *parent, const FileView& fileView, Highlighter& highlighter) :
    QPlainTextEdit(parent),
    parent{parent},
    fileView{fileView},
    shortcutMark{std::make_unique<QShortcut>(Config::markShorcut(), this)},
    lineNumberArea{std::make_unique<LineNumberArea>(this)},
    highlighter{highlighter},
    highlighterDocument{std::make_unique<HighlitherDocument>(*document(), highlighter)}
{
    MEASURE_FUNCTION();
    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    setReadOnly(true);
    setTextInteractionFlags(textInteractionFlags() | Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    setObjectName(QStringLiteral("textBrowser"));
    setFont(Config::getFixedFont());
    calculateLineNumberAreaWidth();
    lineNumberArea->setFont(Config::getFixedFont());

    connect(shortcutMark.get(), &QShortcut::activated, [this](){this->highlightWords();});
    connect(this, &LogArea::blockCountChanged, this, &LogArea::updateLineNumberAreaWidth);
    connect(this, &LogArea::updateRequest, this, &LogArea::updateLineNumberArea);
    connect(this, &LogArea::cursorPositionChanged, this, &LogArea::highlightCurrentLine);

    updateLineNumberAreaWidth(0);

    auto num = 0;
    for (const auto& line: fileView)
    {
        lineNumbers.emplace_back(QString::number(num + 1) + " (" + QString::number(line->lineNum) + ")");
        num++;
    }

    QString buf;
    for (auto i = 0; i < fileView.size(); i++)
    {
        buf.append(fileView[i]->lineText);
    }
    appendPlainText(std::move(buf));
    gotoFirstBlock();
}

LogArea::~LogArea()
{
    std::cout << "deleting LogArea" << std::endl;
}

void LogArea::gotoBlockNum(int blockNum)
{
    QTextCursor cursor(document()->findBlockByLineNumber(blockNum));
    setTextCursor(cursor);
}

void LogArea::gotoLine(int lineNum)
{
    auto lineIt = std::lower_bound(fileView.begin(), fileView.end(), lineNum,
                                   [](const auto& elem, const auto& elem2){return elem->lineNum < elem2;});
    auto blockNum = std::distance(fileView.begin(), lineIt);
    gotoBlockNum(blockNum);
}

void LogArea::gotoFirstBlock()
{
    QTextCursor cursor(document()->firstBlock());
    setTextCursor(cursor);
}

void LogArea::gotoLastBlock()
{
    QTextCursor cursor(document()->lastBlock());
    setTextCursor(cursor);
}

void LogArea::calculateLineNumberAreaWidth()
{
    int digits = 1;
    if (!fileView.empty())
    {
        QString lastEntry = QString::number(fileView.size()) + "(" + QString::number(fileView.back()->lineNum) + ")";
        digits = lastEntry.size();
    }
    auto font = Config::getFixedFont();
    int space = 5+( fontMetrics().averageCharWidth() )* digits;
    lineNumberAreaWidth_ = space;
}

int LogArea::getLineNumberAreaWidth() const
{
    return lineNumberAreaWidth_;
}

void LogArea::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(getLineNumberAreaWidth(), 0, 0, 0);
}


void LogArea::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void LogArea::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), getLineNumberAreaWidth(), cr.height()));
}

void LogArea::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() == Qt::ControlModifier && e->delta() > 0)
    {
       zoomIn();
    }
    else if (e->modifiers() == Qt::ControlModifier && e->delta() < 0)
    {
       zoomOut();
    }
    QPlainTextEdit::wheelEvent(e);
}


void LogArea::mousePressEvent(QMouseEvent *event)
{
    QPlainTextEdit::mousePressEvent(event);
    if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier)
    {
        parent->showLineInFilterSource(getCurrentLineNumber());
    }
}


void LogArea::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    selection.format.setBackground(Config::currentLineColor());
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    setExtraSelections(extraSelections);
}

void LogArea::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea.get());
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = "";
            if (blockNumber <lineNumbers.size())
                number = lineNumbers[blockNumber];
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

QString LogArea::getSelectedText()
{
    return textCursor().selectedText();
}

void LogArea::highlightWords()
{
    highlighter.addToHighLights(textCursor().selectedText().toLower());
    highlighterDocument->highlight(firstVisibleBlock());
}

void LogArea::paintEvent( QPaintEvent* event ) {
    QPlainTextEdit::paintEvent( event );
    highlighterDocument->highlight(firstVisibleBlock());
}

int LogArea::getCurrentBlockNumber()
{
    QTextCursor cursor = textCursor();
    return cursor.blockNumber();
}

int LogArea::getCurrentLineNumber()
{
    if (fileView.empty())
        return -1;
    return fileView[getCurrentBlockNumber()]->lineNum;
}

void LogArea::showFindNoResultsMessage()
{
    QMessageBox* loopMsgBox = new QMessageBox(QMessageBox::Information,
                                              QString("Search"),
                                              QString("No more results found, looping the file"),
                                              QMessageBox::Ok,
                                              this);
    loopMsgBox->show();
}

void LogArea::search(const QString& text, bool backward, bool matchCase, bool regex)
{
    QTextDocument::FindFlags options;
    if (matchCase)
        options |= QTextDocument::FindCaseSensitively;
    if (backward)
        options |= QTextDocument::FindBackward;

    Qt::CaseSensitivity cs = matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;


    auto smartFind = [this, cs, options, regex](const QString& pattern){
        if (regex)
        {
            QRegExp re = QRegExp(pattern, cs);
            return find(re, options);
        }
        else
        {
            return find(pattern, options);
        }
    };

    if (not smartFind(text))
    {
        auto restorePosition =
                [this, cBak = textCursor(), vScrlBak = verticalScrollBar()->value(), hScrlBak = horizontalScrollBar()->value()]()
        {
            setTextCursor(cBak);
            verticalScrollBar()->setValue(vScrlBak);
            horizontalScrollBar()->setValue(hScrlBak);
        };

        showFindNoResultsMessage();
        backward ? gotoLastBlock() : gotoFirstBlock();
        if (not smartFind(text))
        {
            restorePosition();
        }
    }
}
