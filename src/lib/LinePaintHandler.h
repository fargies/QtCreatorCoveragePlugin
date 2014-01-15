#pragma once

#include <QMap>
#include <QColor>

#include "Mark.h"

class QPlainTextEdit;
class LinePaintHandler
{
    QPlainTextEdit *textEdit;
public:
    LinePaintHandler(QPlainTextEdit *textEdit);
    virtual ~LinePaintHandler();

    void render(const QMap<int, Mark *> &marks);

protected:
    virtual QColor getColorForValue(Mark::Type value) const = 0;
};
