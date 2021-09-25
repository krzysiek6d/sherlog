#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QString>
#include <QRegularExpression>
#include <iostream>

class MyHighlighter : public QSyntaxHighlighter
{
public:
    MyHighlighter(QTextDocument *parent, const QString& regex) : QSyntaxHighlighter(parent), regex{regex}
    {}
    void highlightBlock(const QString &text)
    {
        //std::cout << "highlihging " << regex.toStdString() << std::endl;
        QTextCharFormat myClassFormat;
        myClassFormat.setFontWeight(QFont::Bold);
        myClassFormat.setForeground(Qt::darkMagenta);

        int j = 0;

        while ((j = text.indexOf(regex, j)) != -1) {
            setFormat(j, regex.length(), myClassFormat);
            ++j;
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
    QString regex;
};

#endif // MYHIGHLIGHTER_H
