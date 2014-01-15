#pragma once

#include "LinePaintHandler.h"

class LinePainter : public LinePaintHandler
{
public:
    LinePainter(QPlainTextEdit *textEdit);

protected:
    QColor getColorForValue(Mark::Type value) const;
};
