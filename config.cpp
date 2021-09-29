#include "config.h"
#include <QFont>
#include <QFontDatabase>

Config::Config()
{

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
