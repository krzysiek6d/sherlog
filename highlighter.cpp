#include "highlighter.h"
#include "config.h"
#include <iostream>

const int repaintBlockSize = 100;

bool operator<(const QColor& l, const QColor& r)
{
    int lr, lg, lb, rr, rg, rb;
    l.getRgb(&lr, &lg, &lb);
    r.getRgb(&rr, &rg, &rb);
    int lVal = lr << 16 | lg << 8 | lb;
    int rVal = rr << 16 | rg << 8 | rb;
    return lVal < rVal;
}

Highlighter::Highlighter(QTextDocument& document)
    : document_{document}
{
    for(const auto& color: Config::getMarkingColors())
        availableColors.insert({color, true});
}

void Highlighter::addToHighLights(const QString& selectedText)
{
    if (selectedText.length() == 0)
        return;

    auto it = std::find_if(highLightingPatterns.begin(), highLightingPatterns.end(), [&selectedText](auto & elem)
    {
        return selectedText == elem.first;
    });
    if (it != highLightingPatterns.end())
    {
        availableColors[it->second] = true;
        highLightingPatterns.erase(it);
    }
    else
    {
        auto firstAvailableColorIt = std::find_if(availableColors.begin(), availableColors.end(),
                                                [](const auto& elem){
            return elem.second;
        });
        if (firstAvailableColorIt == availableColors.end())
        {
            //assert(!highLightingPatterns.empty());
            auto firstpattern = highLightingPatterns.begin();
            availableColors[firstpattern->second] = true;
            highLightingPatterns.erase(firstpattern);
            firstAvailableColorIt = std::find_if(availableColors.begin(), availableColors.end(),
                                                            [](const auto& elem){
                        return elem.second;
                    });
        }
        firstAvailableColorIt->second = false;
        highLightingPatterns.push_back(std::make_pair(selectedText, firstAvailableColorIt->first));
    }
    std::cout << "selected text: " << selectedText.toStdString() << std::endl;

    auto numOfBlocks = document_.blockCount() / 100 + 1;
    highlightedBlocks = std::vector<int>(numOfBlocks, 0);
}

void Highlighter::highlight(QTextBlock blk)
{
    if (highlightedBlocks.empty())
        return;

    auto blkNum = blk.blockNumber();
    blk = document_.findBlockByLineNumber(blkNum / repaintBlockSize * repaintBlockSize);
    auto max = repaintBlockSize * 2;

    if (highlightedBlocks[blkNum / repaintBlockSize] == 0 )
    {
        highlightedBlocks[blkNum / repaintBlockSize] = 1;

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
