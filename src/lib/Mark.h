#pragma once

#include <QSharedData>
#include <texteditor/basetextmark.h>

class MarkManager;
class Mark : public TextEditor::BaseTextMark
{
public:
    Mark(const QString &fileName, int lineNumber, MarkManager *manager);

    enum Type
    {
        Good,
        NotHit,
        NotBranchCovered
    };

    Type getType() const;
    void setType(Type type);

    void updateLineNumber(int lineNumber);
    void updateBlock(const QTextBlock &block);
    void updateFileName(const QString &fileName);
    void removedFromEditor();

protected:
    MarkManager *manager;
    Type type;
};
