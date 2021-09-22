#include "logarea.h"

#include <QPainter>
#include <QTextBlock>
#include <config.h>
#include <filecontents.h>

CodeEditor::CodeEditor(QWidget *parent, const FileView& fileView) : QPlainTextEdit(parent), fileView{fileView}
{
    std::cout << "CodeEditor 1 " << std::endl;
    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    lineNumberArea = new LineNumberArea(this);

    setFont(Config::getFixedFont());
    lineNumberArea->setFont(Config::getFixedFont());

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);


    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setReadOnly(true);
    setTextInteractionFlags(textInteractionFlags() | Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    setObjectName(QStringLiteral("textBrowser"));

    std::cout << "CodeEditor 2 " << std::endl;

    auto num = 0;
    for (const auto& line: fileView)
    {
        lineNumbers.emplace_back(QString::number(++num) + " (" + QString::number(line->lineNum + 1) + ")");

    }
    std::cout << "CodeEditor 3 " << std::endl;

    for (const auto& line: fileView)
    {
        appendPlainText((*line).lineText);
    }
    std::cout << "CodeEditor 4 " << std::endl;

    calculateLineNumberAreaWidth();
    std::cout << "CodeEditor 5 " << std::endl;

}

void CodeEditor::prepareLineNumbers()
{

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
            QString number = lineNumbers[blockNumber];// QString::number(blockNumber + 1);
            // todo : optimize by having it in vector of qstrings
            //QString number = QString::number(blockNumber + 1) + "(" + QString::number(fileView[blockNumber]->lineNum + 1) + ")";
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