#ifndef FILECONTENTS_H
#define FILECONTENTS_H

#include <QString>
#include <vector>
#include <string>
#include <QFile>
#include <iostream>
#include <QTextStream>

struct Line
{
    int lineNum;
    QString lineText;
};

class FileContents
{
public:
    FileContents(QString filename) : filename_{std::move(filename)}
    {
    }

    bool read()
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

    QString getShortFilename()
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

    const std::vector<Line>& lines() const
    {
        return data_;
    }

    const QString filename() const
    {
        return filename_;
    }
private:
    QString filename_;
    std::vector<Line> data_;
};

class FileView
{
public:
    FileView(const FileContents& fileContents) : fileContents{fileContents}
    {
        std::cout << "c-tor of FileView" << std::endl;
        for(auto it = fileContents.lines().cbegin(); it != fileContents.lines().cend(); it = std::next(it))
        {
            visibleLines.emplace_back(it);
        }
    }

    // TODO: regex support
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

    int size() const
    {
        return visibleLines.size();
    }

private:
    const FileContents& fileContents;
    std::vector<typename std::vector<Line>::const_iterator> visibleLines;
};

#endif // FILECONTENTS_H
