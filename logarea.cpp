#include "logarea.h"

#include <QPainter>
#include <QTextBlock>
#include <config.h>
#include <filecontents.h>
#include "timer.h"
#include <QShortcut>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <algorithm>
#include <ranges>
#include <cassert>
#include <thread>
#include <future>
#include <QTimer>

CodeEditor::CodeEditor(QWidget *parent, const FileView& fileView) : QPlainTextEdit(parent), fileView{fileView}, highlighter(nullptr)
{
    MEASURE_FUNCTION();
    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    setReadOnly(true);
    setTextInteractionFlags(textInteractionFlags() | Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    setObjectName(QStringLiteral("textBrowser"));
    setFont(Config::getFixedFont());
    calculateLineNumberAreaWidth();

    availableColors.insert(std::make_pair(Qt::GlobalColor::lightGray, true));
    availableColors.insert(std::make_pair(Qt::GlobalColor::green, true));
    availableColors.insert(std::make_pair(Qt::GlobalColor::blue, true));
    availableColors.insert(std::make_pair(Qt::GlobalColor::cyan, true));
    availableColors.insert(std::make_pair(Qt::GlobalColor::red, true));
    availableColors.insert(std::make_pair(Qt::GlobalColor::yellow, true));
    availableColors.insert(std::make_pair(Qt::GlobalColor::darkYellow, true));


    QShortcut *shortcutFind = new QShortcut(Config::markShorcut(), this); // rememver to delete
    QObject::connect(shortcutFind, &QShortcut::activated, [this](){this->highlightWords();});


    lineNumberArea = new LineNumberArea(this);
    lineNumberArea->setFont(Config::getFixedFont());

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    auto num = 0;
    for (const auto& line: fileView)
    {
        lineNumbers.emplace_back(QString::number(num + 1) + " (" + QString::number(line->lineNum) + ")");
        num++;
    }

    QString buf;
    for (auto i = 0; i < fileView.getNumOfLines(); i++)
    {
        buf.append(fileView[i]->lineText);
    }
    appendPlainText(std::move(buf));
    buf.clear();
}

void CodeEditor::calculateLineNumberAreaWidth()
{
    int digits = 1;
    if (!fileView.empty())
    {
        QString lastEntry = QString::number(fileView.getNumOfLines()) + "(" + QString::number(fileView.back()->lineNum) + ")";
        digits = lastEntry.size();
    }
    auto font = Config::getFixedFont();
    int space = 5+( fontMetrics().averageCharWidth() )* digits;
    lineNumberAreaWidth_ = space;
}

int CodeEditor::getLineNumberAreaWidth() const
{
    return lineNumberAreaWidth_;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(getLineNumberAreaWidth(), 0, 0, 0);
}


void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), getLineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    setExtraSelections(extraSelections);
}

void CodeEditor::fastHighlight()
{

    auto highlight = [this](QTextBlock blk, int num)
    {
        int i = 0;
        while (blk.isValid() && i < num) {
            highlighter->rehighlightBlock(blk);
            i++;
            blk = blk.next();
        }
    };

    auto blk = firstVisibleBlock();
    auto max = 200;

    highlight(blk, max);
//    QTimer::singleShot(2, [this, highlight]()
//    {
//        highlight(document()->firstBlock(), document()->blockCount());
//    });
    //highlight(document()->firstBlock(), document()->blockCount());


    /////////////////////////////////////////
//auto blk = firstVisibleBlock();
//auto blk = document()->begin();

//    auto max = 200;
//    //auto max = blockCount();

//    int i = 0;
//    while (blk.isValid() && i < max) {

//        auto text = blk.text();

//        QTextCharFormat clearFormat;
//        QTextCursor cursor(blk);
//        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
//        cursor.setCharFormat(clearFormat);
//        for (const auto& [pattern, color]: highLightingPatterns)
//        {
//            QTextCharFormat myClassFormat;
//            myClassFormat.setFontWeight(QFont::Bold);
//            myClassFormat.setBackground(color);

//            int j = 0;
//            while ((j = text.indexOf(pattern, j, Qt::CaseInsensitive)) != -1) {

//                QTextCursor tempCursor(blk);
//                tempCursor.setPosition(blk.begin().fragment().position() + j);
//                tempCursor.setPosition(blk.begin().fragment().position() + j + pattern.length(), QTextCursor::KeepAnchor);
//                tempCursor.setCharFormat(myClassFormat);
//                ++j;
//            }

//        }
//        i++;
//        blk = blk.next();
//    }

}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
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

QString CodeEditor::getSelectedText()
{
    return textCursor().selectedText();
}

void CodeEditor::highlightWords()
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

    if (!highlighter)
        highlighter.reset(new MyHighlighter(document(), highLightingPatterns));
    else
    {
        highlighter->setPatterns(highLightingPatterns);
        highlighter->rehighlight();
    }

    //fastHighlight();


}


int CodeEditor::getCurrentLineNumber()
{
    QTextCursor cursor = textCursor();
    return cursor.blockNumber();
}
