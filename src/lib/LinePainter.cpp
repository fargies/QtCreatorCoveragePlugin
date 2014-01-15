#include "LinePainter.h"

LinePainter::LinePainter(QPlainTextEdit *textEdit) :
    LinePaintHandler(textEdit)
{
}

QColor LinePainter::getColorForValue(Mark::Type value) const
{
    switch (value)
    {
    case Mark::Good:
        return QColor(0, 255, 0, 60);
    case Mark::NotBranchCovered:
        return QColor(255, 144, 0, 60);
    default:
    case Mark::NotHit:
        return QColor(255, 0, 0, 60);
    }
}
