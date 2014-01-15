#include "LineCleaner.h"

LineCleaner::LineCleaner(QPlainTextEdit *textEdit) :
    LinePaintHandler(textEdit)
{
}

QColor LineCleaner::getColorForValue(Mark::Type value) const
{
    Q_UNUSED(value);
    return QColor(Qt::white);
}
