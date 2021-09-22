#ifndef TABWITHFILENAME_H
#define TABWITHFILENAME_H

#include <QWidget>
#include <filecontents.h>
namespace Ui {
class tabWithFilename;
}

class tabWithFilename : public QWidget
{
    Q_OBJECT

public:
    explicit tabWithFilename(QWidget *parent, const FileContents& fileContents);
    ~tabWithFilename();

private:

    Ui::tabWithFilename *ui;
    FileView fileView;
};

#endif // TABWITHFILENAME_H
