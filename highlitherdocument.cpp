#include "highlitherdocument.h"
#include <iostream>

const int repaintBlockSize = 100;

HighlitherDocument::HighlitherDocument(QTextDocument& document, Highlighter& highlighter)
    : document_{document}
    , highlighter_{highlighter}
{
    highlighter_.subscribeDocument(this);
    clearHighlightingCache();
}

HighlitherDocument::~HighlitherDocument()
{
    highlighter_.unsubscribeDocument(this);
}

void HighlitherDocument::clearHighlightingCache()
{
    std::cout << "clearing cache" << std::endl;
    auto numOfBlocks = document_.blockCount() / repaintBlockSize + 1;
    highlightedBlocks = std::vector<int>(numOfBlocks, 0);
}


void HighlitherDocument::highlight(QTextBlock blk)
{
    if (highlightedBlocks.empty())
        return;

    auto blkNum = blk.blockNumber();
    blk = document_.findBlockByLineNumber(blkNum / repaintBlockSize * repaintBlockSize);
    auto max = repaintBlockSize * 2;

    if (highlightedBlocks[blkNum / repaintBlockSize] == 0 )
    {
        highlightedBlocks[blkNum / repaintBlockSize] = 1;

        const auto& highLightingPatterns = highlighter_.getHighLightingPatterns();

        int i = 0;
        while (blk.isValid() && i < max) {

            auto text = blk.text();

            QTextCharFormat clearFormat;
            QTextCursor cursor(blk);
            cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            cursor.setCharFormat(clearFormat);
            for (const auto& [pattern, color]: highLightingPatterns)
            {
                QTextCharFormat myClassFormat;
                myClassFormat.setFontWeight(QFont::Bold);
                myClassFormat.setBackground(color);

                int j = 0;
                while ((j = text.indexOf(pattern, j, Qt::CaseInsensitive)) != -1) {

                    QTextCursor tempCursor(blk);
                    tempCursor.setPosition(blk.begin().fragment().position() + j);
                    tempCursor.setPosition(blk.begin().fragment().position() + j + pattern.length(), QTextCursor::KeepAnchor);
                    tempCursor.setCharFormat(myClassFormat);
                    ++j;
                }

            }
            i++;
            blk = blk.next();
        }
    }
}
