#ifndef CONFIG_H
#define CONFIG_H

#include <QFont>
#include <QKeySequence>
#include <vector>
#include <QColor>

class Config
{
public:
    Config();
    static QFont getFixedFont();
    static QFont getNormalFont();
    static const std::vector<QColor>& getMarkingColors();

    static QKeySequence findShorcut();
    static QKeySequence findNextShorcut();
    static QKeySequence findPrevShorcut();
    static QKeySequence grepShorcut();
    static QKeySequence markShorcut();
    static QKeySequence gotoLineShorcut();
    static QKeySequence bookmarkShorcut();
};

#endif // CONFIG_H
