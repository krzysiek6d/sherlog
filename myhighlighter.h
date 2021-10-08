#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H

#include <QString>
#include <QRegularExpression>
#include <iostream>
#include <vector>
#include <set>
#include <QCursor>
#include <QTextCursor>
#include <QSyntaxHighlighter>

//class FastHighlighter{
//public:
//    void rehighlightDocument(QTextDocument* doc)
//    {
//        QTextCursor cursor(doc);
//        rehighlight(cursor, QTextCursor::EndOfBlock);
//    }
//    void rehighlightBlock(const QTextBlock &block)
//    {
//        QTextCursor cursor(block);
//        rehighlight(cursor, QTextCursor::End);
//    }
//    void rehighlight(QTextCursor &cursor, QTextCursor::MoveOperation operation)
//    {
//        cursor.beginEditBlock();
//        int from = cursor.position();
//        cursor.movePosition(operation);
//        //reformatBlocks(from, 0, cursor.position() - from);
//        cursor.endEditBlock();
//    }

//};

class MyHighlighter : public QSyntaxHighlighter
{
public:
    MyHighlighter(QTextDocument *parent, const std::vector<std::pair<QString, Qt::GlobalColor>>& patterns) : QSyntaxHighlighter(parent), regex{patterns}
    {

    }

    void setPatterns(const std::vector<std::pair<QString, Qt::GlobalColor>>& patterns)
    {
        regex = patterns;
    }

    void highlightBlock(const QString &text)
    {
        for (const auto& [pattern, color]: regex)
        {
            QTextCharFormat myClassFormat;
            myClassFormat.setFontWeight(QFont::Bold);
            myClassFormat.setBackground(color);

            int j = 0;
            while ((j = text.indexOf(pattern, j, Qt::CaseInsensitive)) != -1) {
                setFormat(j, pattern.length(), myClassFormat);
                ++j;
            }
        }
        /*QRegularExpression expression(regex);
        QRegularExpressionMatchIterator i = expression.globalMatch(text);
        while (i.hasNext())
        {
          QRegularExpressionMatch match = i.next();
          setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);
        }*/
    }
    ~MyHighlighter()
    {
        std::cout << "destroying highlighter" << std::endl;
    }
private:
    std::vector<std::pair<QString, Qt::GlobalColor>> regex;
};

#endif // MYHIGHLIGHTER_H
