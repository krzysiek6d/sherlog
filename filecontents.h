#ifndef FILECONTENTS_H
#define FILECONTENTS_H

#include <QString>
#include <vector>
#include <string>
#include <QFile>
#include <iostream>

struct FileContents
{
    FileContents() {}
    QString filename;
    std::vector<QString> data;
    QString getShortFilename()
    {
        std::cout << "len of filename: " << filename.size() << std::endl;
        auto file = filename.toStdString();
        std::cout << "file location" << file << std::endl;
        auto lastslash = file.find_last_of('/');
        std::cout << "last slash found on pos: " << lastslash << std::endl;
        if (file.find_last_of('/') != std::string::npos)
        {
            auto shorterFilename = file.substr(lastslash+1);
            return QString::fromStdString(shorterFilename);
        }
        return filename;
    }
};

#endif // FILECONTENTS_H
