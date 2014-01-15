#pragma once

#include "LinePaintHandler.h"

class LineCleaner : public LinePaintHandler
{
public:
    LineCleaner(QPlainTextEdit *textEdit);

protected:
    QColor getColorForValue(Mark::Type value) const;
};
