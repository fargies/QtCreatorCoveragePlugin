#include "LinePaintHandler.h"

#include <QPlainTextEdit>
#include <QTextCursor>
#include <QDebug>
#include <texteditor/basetexteditor.h>

LinePaintHandler::LinePaintHandler(QPlainTextEdit *textEdit) :
    textEdit(textEdit)
{
}

LinePaintHandler::~LinePaintHandler()
{
}

void LinePaintHandler::render(const QMap<int, Mark *> &marks)
{
    QTextCursor cursor = textEdit->cursorForPosition(QPoint(0, 0));
    int startBlockNumber = cursor.blockNumber();
    QTextCursor endCursor = textEdit->cursorForPosition(textEdit->viewport()->rect().bottomLeft());
    int endBlockNumber = endCursor.blockNumber();

    QMap<int, Mark *>::const_iterator beginIterator = marks.lowerBound(startBlockNumber);
    QMap<int, Mark *>::const_iterator endIterator = marks.upperBound(endBlockNumber + 1);
    QList<QTextEdit::ExtraSelection> selections;
    int prevPos = startBlockNumber;
    for (auto it = beginIterator; it != endIterator; ++it) {
        int pos = it.key();
        if (pos == prevPos || !it.value())
            continue;

        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(getColorForValue(it.value()->getType()));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, pos - prevPos - 1);
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
        selection.cursor = cursor;
        selections.append(selection);
        prevPos = pos;
    }

    /* when using a BaseTextEditor (from the texteditor plugin) ExtraSelections
     * get overriden by the pluggin really often, clearing ours, to prevent
     * that we the plugin's API with a selection type that's not really meant
     * for that */
    TextEditor::BaseTextEditorWidget *baseTextEdit = qobject_cast<TextEditor::BaseTextEditorWidget *>(textEdit);
    if (baseTextEdit) {
        baseTextEdit->setExtraSelections(TextEditor::BaseTextEditorWidget::DebuggerExceptionSelection, selections);
    } else {
        textEdit->setExtraSelections(selections);
    }
}
