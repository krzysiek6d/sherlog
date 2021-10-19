#include "config.h"
#include <QFont>
#include <QFontDatabase>

Config::Config()
{

}

const std::vector<QColor>& Config::getMarkingColors()
{
    static std::vector<QColor> colors;
    colors.push_back(QColor(154, 222, 235));
    colors.push_back(QColor(203, 171, 199));
    colors.push_back(QColor(237, 212, 0));
    colors.push_back(QColor(210, 139, 139));
    colors.push_back(QColor(139, 210, 168));

    return colors;
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
