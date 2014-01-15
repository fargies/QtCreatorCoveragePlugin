#pragma once

#include <QObject>

#include <QHash>
#include <QMap>

#include "Mark.h"

class Mark;
class MarkManager : public QObject
{
    Q_OBJECT

public:
    explicit MarkManager(QObject *parent = 0);
    ~MarkManager();

    typedef QMap<int, Mark *> FileMarks;

    void removeAllMarks();

    Mark *addMark(const QString &fileName, int lineNumber);

    FileMarks getMarks(const QString &fileName) const;

protected:
    QHash<QString, FileMarks> marks;
};
