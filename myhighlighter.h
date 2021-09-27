#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QString>
#include <QRegularExpression>
#include <iostream>
#include <vector>
#include <set>

class MyHighlighter : public QSyntaxHighlighter
{
public:
    MyHighlighter(QTextDocument *parent, const std::vector<std::pair<QString, Qt::GlobalColor>>& regex) : QSyntaxHighlighter(parent), regex{regex}
    {}
    void highlightBlock(const QString &text)
    {
        //std::cout << "highlihging " << regex.toStdString() << std::endl;
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
