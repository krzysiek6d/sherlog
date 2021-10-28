#ifndef HIGHLITHERDOCUMENT_H
#define HIGHLITHERDOCUMENT_H

#include <QTextDocument>
#include <vector>
#include <highlighter.h>
#include <memory>

class HighlitherDocument
{
public:
    HighlitherDocument(QTextDocument& document, std::shared_ptr<Highlighter> highlighter);
    void clearHighlightingCache();
    void highlight(QTextBlock blk);
    ~HighlitherDocument();
private:
    QTextDocument& document_;
    std::shared_ptr<Highlighter> highlighter_;
    std::vector<int> highlightedBlocks;
};

#endif // HIGHLITHERDOCUMENT_H
