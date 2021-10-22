#include <filecontents.h>
#include <QFile>
#include <QFileDevice>
#include <QTextStream>
#include <QString>
#include <iostream>

FileContents::FileContents(QString filename) : filename_{std::move(filename)}
{
}

bool FileContents::read()
{
    std::cout << "opening " << filename_.toStdString() << std::endl;
    QFile file{filename_};

    if (file.open(QFileDevice::ReadOnly | QFileDevice::Text))
    {
        std::cout << "reading " << filename_.toStdString() << std::endl;
        QTextStream in(&file);
        int lineNum = 1;
        while (!in.atEnd())
        {
            QString line = file.readLine();
            Line l {lineNum, line};
            data_.emplace_back(l);
            lineNum++;
        }
        file.close();
        return true;
    }
    return false;
}

QString FileContents::getShortFilename() const
{
    std::cout << "len of filename: " << filename_.size() << std::endl;
    auto file = filename_.toStdString();
    auto lastslash = file.find_last_of('/');
    if (file.find_last_of('/') != std::string::npos)
    {
        auto shorterFilename = file.substr(lastslash+1);
        return QString::fromStdString(shorterFilename);
    }
    return filename_;
}


const std::vector<Line>& FileContents::lines() const
{
    return data_;
}


const QString FileContents::filename() const
{
    return filename_;
}
