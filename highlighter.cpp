#include "highlighter.h"
#include "config.h"
#include <iostream>
#include "highlitherdocument.h"

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

Highlighter::Highlighter()
{
    for(const auto& color: Config::getMarkingColors())
        availableColors.insert({color, true});
}

void Highlighter::subscribeDocument(HighlitherDocument* ptr)
{
    documents.insert(ptr);
}

void Highlighter::unsubscribeDocument(HighlitherDocument* ptr)
{
    documents.erase(ptr);
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

    for (auto doc: documents)
        doc->clearHighlightingCache();
}

const std::vector<std::pair<QString, QColor>>& Highlighter::getHighLightingPatterns() const
{
    return highLightingPatterns;
}

