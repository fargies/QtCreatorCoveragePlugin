#pragma once

#include <QObject>
#include <QMap>

#include "Mark.h"

namespace Core {class IEditor;}
namespace TextEditor {class ITextEditor;}

class ProjectTreeManager;
class MarkManager;
class QAction;
class QPlainTextEdit;
class Visualizer : public QObject
{
    Q_OBJECT
    ProjectTreeManager *projectTreeManager;
    MarkManager *markManager;
    QAction *renderAction;

public:
    Visualizer(ProjectTreeManager *projectTreeManager, QAction *renderAction, QObject *parent = 0);

public Q_SLOTS:
    void refreshMarks();
    void renderCoverage();
    /**
     * @brief render coverage for current editor only.
     */
    void renderCurrentCoverage();

private Q_SLOTS:
    void renderCoverage(Core::IEditor *editor) const;
    void clearCoverage(Core::IEditor *editor) const;
    void bindEditorToPainting(Core::IEditor *editor);

    void repaintMarks(bool isRender);

private:
    TextEditor::ITextEditor *currentTextEditor() const;
    QMap<int, Mark *> getLineCoverage(Core::IEditor *editor) const;
};
