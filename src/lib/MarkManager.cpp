#include "MarkManager.h"

MarkManager::MarkManager(QObject *parent) :
    QObject(parent)
{
}

MarkManager::~MarkManager()
{
    removeAllMarks();
}

void MarkManager::removeAllMarks()
{
    foreach (const FileMarks &m, marks) {
        qDeleteAll(m);
    }
    marks.clear();
}

Mark *MarkManager::addMark(const QString &fileName, int lineNumber)
{
    Mark *&mark = marks[fileName][lineNumber];

    if (!mark)
    {
        mark = new Mark(fileName, lineNumber, this);
        mark->init();
    }

    return mark;
}

QMap<int, Mark *> MarkManager::getMarks(const QString &fileName) const
{
    return marks.value(fileName);
}
