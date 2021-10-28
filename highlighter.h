#pragma once

#include <QTextDocument>
#include <QString>
#include <vector>
#include <map>
#include <QColor>
#include <QTextBlock>
#include <set>

class HighlitherDocument;

class Highlighter
{
public:
    Highlighter();
    ~Highlighter();
    void addToHighLights(const QString& text);
    const std::vector<std::pair<QString, QColor>>& getHighLightingPatterns() const;
    void subscribeDocument(HighlitherDocument* ptr);
    void unsubscribeDocument(HighlitherDocument* ptr);
private:
    std::map<QColor, bool> availableColors;
    std::vector<std::pair<QString, QColor>> highLightingPatterns;
    std::set<HighlitherDocument*> documents;
};
