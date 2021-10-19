#pragma once

#include <QPlainTextEdit>
#include "filecontents.h"
#include <memory>
#include <QShortcut>
#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QShortcut;
QT_END_NAMESPACE

class LineNumberArea;
class MyTab;



class LogArea : public QPlainTextEdit
{
    Q_OBJECT

public:
    LogArea(MyTab *parent, const FileView& fileView);
    ~LogArea();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int getLineNumberAreaWidth() const;
    int getCurrentBlockNumber();
    int getCurrentLineNumber();
    QString getSelectedText();
    void gotoBlockNum(int blockNum);
    void gotoLine(int lineNum);
    void gotoFirstBlock();
    void gotoLastBlock();
    void search(const QString& text, bool backward, bool matchCase, bool regex);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    void calculateLineNumberAreaWidth();
    void highlightWords();
    void showFindNoResultsMessage();

    MyTab *parent;
    const FileView& fileView;
    std::unique_ptr<QShortcut> shortcutMark;
    std::unique_ptr<QWidget> lineNumberArea;
    std::unique_ptr<Highlighter> highlighter;

    int lineNumberAreaWidth_;
    std::vector<QString> lineNumbers;

};


class LineNumberArea : public QWidget
{
public:
    LineNumberArea(LogArea *editor) : QWidget(editor), logArea(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(logArea->getLineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        logArea->lineNumberAreaPaintEvent(event);
    }

private:
    LogArea *logArea;
};
