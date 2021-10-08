#ifndef FILECONTENTS_H
#define FILECONTENTS_H

#include <QString>
#include <vector>
#include <string>
#include <QFile>
#include <iostream>

struct Line
{
    int lineNum;
    QString lineText;
};

struct FileContents
{
    FileContents() {}
    QString filename;
    std::vector<Line> data;
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

class FileView
{
public:
    FileView(const FileContents& fileContents) : fileContents{fileContents}
    {
        std::cout << "c-tor of FileView" << std::endl;
        for(auto it = std::begin(fileContents.data); it != std::end(fileContents.data); it = std::next(it))
        {
            visibleLines.emplace_back(it);
        }
    }

    void filter(const QString& substring, bool matchCase)
    {
        std::vector<typename std::vector<Line>::const_iterator> newVisibleLines;
        Qt::CaseSensitivity cs = matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive;
        for(auto it : visibleLines)
        {
            const auto& line = it->lineText;
            if (line.contains(substring, cs))
            {
                newVisibleLines.emplace_back(it);
            }
        }
        visibleLines = newVisibleLines;
    }

    auto begin() const
    {
        return std::begin(visibleLines);
    }


    auto end() const
    {
        return std::end(visibleLines);
    }

    auto back() const
    {
        return visibleLines.back();
    }

    auto operator[](int k) const
    {
        return visibleLines[k];
    }

    auto empty() const
    {
        return visibleLines.empty();
    }

    int getNumOfLines() const
    {
        return visibleLines.size();
    }


private:

    const FileContents& fileContents;
    std::vector<typename std::vector<Line>::const_iterator> visibleLines;
};

#endif // FILECONTENTS_H
