#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QTextDocument>
#include <QString>
#include <vector>
#include <map>
#include <QColor>
#include <QTextBlock>

class Highlighter
{
public:
    Highlighter(QTextDocument& document);
    void addToHighLights(const QString& text);
    void highlight(QTextBlock blk);
private:
    QTextDocument& document_;

    std::map<QColor, bool> availableColors;
    std::vector<std::pair<QString, QColor>> highLightingPatterns;
    std::vector<int> highlightedBlocks;
};

#endif // HIGHLIGHTER_H
