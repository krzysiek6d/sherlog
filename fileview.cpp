#include "fileview.h"
#include "filecontents.h"
#include <QString>
#include <QRegularExpression>

FileView::FileView(const FileContents& fileContents) : fileContents{fileContents}
{
    for(auto it = fileContents.lines().cbegin(); it != fileContents.lines().cend(); it = std::next(it))
    {
        visibleLines.emplace_back(it);
    }
}

void FileView::filter(const QString& substring, bool matchCase, bool reverse, bool regex)
{
    if (regex)
        return filterRegex(substring, reverse, matchCase);
    else
        return filterPlainText(substring, reverse, matchCase);
}


void FileView::filterPlainText(const QString& pattern, bool reverse, bool matchCase)
{
    std::vector<typename std::vector<Line>::const_iterator> newVisibleLines;

    Qt::CaseSensitivity cs = matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;

    for(auto it : visibleLines)
    {
        const auto& line = it->lineText;
        if (reverse xor line.contains(pattern, cs))
        {
            newVisibleLines.emplace_back(it);
        }
    }
    visibleLines = newVisibleLines;
}

void FileView::filterRegex(const QString& pattern, bool reverse, bool matchCase)
{
    std::vector<typename std::vector<Line>::const_iterator> newVisibleLines;

    QRegularExpression::PatternOptions reOptions;
    if (not matchCase)
        reOptions |= QRegularExpression::CaseInsensitiveOption;
    auto re = QRegularExpression(pattern, reOptions);

    for(auto it : visibleLines)
    {
        const auto& line = it->lineText;
        if (reverse xor re.match(line).hasMatch())
        {
            newVisibleLines.emplace_back(it);
        }
    }
    visibleLines = newVisibleLines;
}
