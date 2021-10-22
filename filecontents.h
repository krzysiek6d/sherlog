#pragma once

#include <QString>
#include <vector>
#include <line.h>

class FileContents
{
public:
    FileContents(QString filename);
    bool read();
    QString getShortFilename() const;
    const std::vector<Line>& lines() const;
    const QString filename() const;
private:
    QString filename_;
    std::vector<Line> data_;
};
