#include "Mark.h"

#include "MarkManager.h"

Mark::Mark(const QString &fileName, int lineNumber, MarkManager *manager) :
    BaseTextMark(fileName, lineNumber),
    manager(manager),
    type(Good)
{
    setPriority(TextEditor::ITextMark::NormalPriority);
    setIcon(QIcon(QLatin1String(":/cov/icons/good.png")));
}

Mark::Type Mark::getType() const
{
    return type;
}

void Mark::setType(Mark::Type type)
{
    if (type == this->type)
        return;

    this->type = type;
    switch (type)
    {
    case Good:
        setIcon(QIcon(QLatin1String(":/cov/icons/good.png"))); break;
    case NotHit:
        setIcon(QIcon(QLatin1String(":/cov/icons/nothit.png"))); break;
    case NotBranchCovered:
        setIcon(QIcon(QLatin1String(":/cov/icons/notbranchcovered.png"))); break;
    }
}

void Mark::updateLineNumber(int lineNumber)
{
    BaseTextMark::updateLineNumber(lineNumber);
}

void Mark::updateBlock(const QTextBlock &block)
{
    BaseTextMark::updateBlock(block);
}

void Mark::updateFileName(const QString &fileName)
{
    BaseTextMark::updateFileName(fileName);
}

void Mark::removedFromEditor()
{
    BaseTextMark::removedFromEditor();
}
