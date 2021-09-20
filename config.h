#ifndef CONFIG_H
#define CONFIG_H

#include <QFont>

class Config
{
public:
    Config();
    static QFont getFixedFont();
    static QFont getNormalFont();
};

#endif // CONFIG_H
