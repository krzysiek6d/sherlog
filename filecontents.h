#ifndef FILECONTENTS_H
#define FILECONTENTS_H

#include <QString>
#include <vector>

struct FileContents
{
    FileContents() {}
    std::vector<QString> data;
};

#endif // FILECONTENTS_H
