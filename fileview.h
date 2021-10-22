#pragma once

#include <QString>
#include <vector>
#include <line.h>

class FileContents;

class FileView
{
public:
    FileView(const FileContents& fileContents);
    void filter(const QString& substring, bool matchCase, bool reverse, bool regex);

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
    void filterPlainText(const QString& pattern, bool reverse, bool matchCase);
    void filterRegex(const QString& pattern, bool reverse, bool matchCase);

    const FileContents& fileContents;
    std::vector<typename std::vector<Line>::const_iterator> visibleLines;
};
