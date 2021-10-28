#ifndef HIGHLITHERDOCUMENT_H
#define HIGHLITHERDOCUMENT_H

#include <QTextDocument>
#include <vector>
#include <highlighter.h>

class HighlitherDocument
{
public:
    HighlitherDocument(QTextDocument& document, Highlighter& highlighter);
    void clearHighlightingCache();
    void highlight(QTextBlock blk);
    ~HighlitherDocument();
private:
    QTextDocument& document_;
    Highlighter& highlighter_;
    std::vector<int> highlightedBlocks;
};

#endif // HIGHLITHERDOCUMENT_H
