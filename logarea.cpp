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

const int repaintBlockSize = 100;

bool operator<(const QColor& l, const QColor& r)
{
    int lr, lg, lb, rr, rg, rb;
    l.getRgb(&lr, &lg, &lb);
    r.getRgb(&rr, &rg, &rb);
    int lVal = lr << 16 | lg << 8 | lb;
    int rVal = rr << 16 | rg << 8 | rb;
    return lVal < rVal;
}


LogArea::LogArea(MyTab *parent, const FileView& fileView) :
    QPlainTextEdit(parent),
    parent{parent},
    fileView{fileView},
    shortcutMark{std::make_unique<QShortcut>(Config::markShorcut(), this)},
    lineNumberArea{std::make_unique<LineNumberArea>(this)}
{
    MEASURE_FUNCTION();
    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    setReadOnly(true);
    setTextInteractionFlags(textInteractionFlags() | Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    setObjectName(QStringLiteral("textBrowser"));
    setFont(Config::getFixedFont());
    calculateLineNumberAreaWidth();
    lineNumberArea->setFont(Config::getFixedFont());

    for(const auto& color: Config::getMarkingColors())
        availableColors.insert({color, true});

    connect(shortcutMark.get(), &QShortcut::activated, [this](){this->highlightWords();});
    connect(this, &LogArea::blockCountChanged, this, &LogArea::updateLineNumberAreaWidth);
    connect(this, &LogArea::updateRequest, this, &LogArea::updateLineNumberArea);
    connect(this, &LogArea::cursorPositionChanged, this, &LogArea::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

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

    auto numOfBlocks = fileView.size() / repaintBlockSize + 1;
    highlightedBlocks = std::vector<int>(numOfBlocks, 0);
    isHiglightingConnected = false;
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
    if ((e->modifiers() == Qt::ControlModifier) && (e->delta() > 0))
    {
       zoomIn();
    }
    else if ((e->modifiers() == Qt::ControlModifier) && (e->delta() < 0))
    {
       zoomOut();
    }
    QPlainTextEdit::wheelEvent(e);
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

void LogArea::fastHighlight()
{
    auto blk = firstVisibleBlock();
    auto blkNum = blk.blockNumber();
    blk = document()->findBlockByLineNumber(blkNum / repaintBlockSize * repaintBlockSize);
    auto max = repaintBlockSize * 2;

    if (highlightedBlocks[blkNum / repaintBlockSize] == 0 )
    {
        highlightedBlocks[blkNum / repaintBlockSize] = 1;

        int i = 0;
        while (blk.isValid() && i < max) {

            auto text = blk.text();

            QTextCharFormat clearFormat;
            QTextCursor cursor(blk);
            cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            cursor.setCharFormat(clearFormat);
            for (const auto& [pattern, color]: highLightingPatterns)
            {
                QTextCharFormat myClassFormat;
                myClassFormat.setFontWeight(QFont::Bold);
                myClassFormat.setBackground(color);

                int j = 0;
                while ((j = text.indexOf(pattern, j, Qt::CaseInsensitive)) != -1) {

                    QTextCursor tempCursor(blk);
                    tempCursor.setPosition(blk.begin().fragment().position() + j);
                    tempCursor.setPosition(blk.begin().fragment().position() + j + pattern.length(), QTextCursor::KeepAnchor);
                    tempCursor.setCharFormat(myClassFormat);
                    ++j;
                }

            }
            i++;
            blk = blk.next();
        }
    }
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
    MEASURE_FUNCTION();
    auto selectedText = textCursor().selectedText().toLower();

    auto it = std::find_if(highLightingPatterns.begin(), highLightingPatterns.end(),
                           [&selectedText](auto & elem)
    {
        return selectedText == elem.first;
    });
    if (it != highLightingPatterns.end())
    {
        availableColors[it->second] = true;
        highLightingPatterns.erase(it);
    }
    else
    {
        auto firstAvailableColorIt = std::find_if(availableColors.begin(), availableColors.end(),
                                                [](const auto& elem){
            return elem.second;
        });
        if (firstAvailableColorIt == availableColors.end())
        {
            assert(!highLightingPatterns.empty());
            auto firstpattern = highLightingPatterns.begin();
            availableColors[firstpattern->second] = true;
            highLightingPatterns.erase(firstpattern);
            firstAvailableColorIt = std::find_if(availableColors.begin(), availableColors.end(),
                                                            [](const auto& elem){
                        return elem.second;
                    });
        }
        firstAvailableColorIt->second = false;
        highLightingPatterns.push_back(std::make_pair(selectedText, firstAvailableColorIt->first));
    }
    std::cout << "selected text: " << selectedText.toStdString() << std::endl;

    isHiglightingConnected = true;

    auto numOfBlocks = fileView.size() / 100 + 1;
    highlightedBlocks = std::vector<int>(numOfBlocks, 0);

    fastHighlight();
}

void LogArea::paintEvent( QPaintEvent* event ) {
    QPlainTextEdit::paintEvent( event );
    if (isHiglightingConnected)
        fastHighlight();
}

int LogArea::getCurrentBlockNumber()
{
    QTextCursor cursor = textCursor();
    return cursor.blockNumber();
}

int LogArea::getCurrentLineNumber()
{
    return fileView[getCurrentBlockNumber()]->lineNum;
}

void LogArea::mousePressEvent(QMouseEvent *event)
{
    QPlainTextEdit::mousePressEvent(event);
    if (event->button() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier)
    {
        parent->showLineInFilterSource(getCurrentLineNumber());
    }
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
