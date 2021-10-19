#include "config.h"
#include <QFont>
#include <QFontDatabase>

Config::Config()
{

}

const std::vector<QColor>& Config::getMarkingColors()
{
    static std::vector<QColor> colors{
        QColor(240, 232, 205),
        QColor(192, 213, 185),
        QColor(253, 202, 162),
        QColor(255, 250, 129),
        QColor(224, 243, 176),
        QColor(111, 183, 214),
        QColor(191, 213, 232),
        QColor(117, 137, 191),
        QColor(165, 137, 193),
        QColor(253, 222, 238),
        QColor(249, 140, 182)};
    return colors;
}

QColor Config::currentLineColor()
{
    return QColor(240, 240, 240);
}

QFont Config::getFixedFont()
{
    auto font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPointSize(8);
    return font;
}

QFont Config::getNormalFont()
{
    QFont font;
    font.setFamily(QStringLiteral("DejaVu Sans"));
    font.setPointSize(8);
    return font;
}

QKeySequence Config::findShorcut()
{
    return QKeySequence("Ctrl+f");
}

QKeySequence Config::findNextShorcut()
{
    return QKeySequence("Ctrl+n");
}

QKeySequence Config::findPrevShorcut()
{
    return QKeySequence("Ctrl+Shift+n");
}

QKeySequence Config::grepShorcut()
{
    return QKeySequence("Ctrl+g");
}

QKeySequence Config::markShorcut()
{
    return QKeySequence("Ctrl+m");
}

QKeySequence Config::gotoLineShorcut()
{
    return QKeySequence("Ctrl+l");
}

QKeySequence Config::bookmarkShorcut()
{
    return QKeySequence("Ctrl+b");
}
