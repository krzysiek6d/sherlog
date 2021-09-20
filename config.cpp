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
