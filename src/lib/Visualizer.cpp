#include "Visualizer.h"

#include "Mark.h"
#include "MarkManager.h"
#include "Node.h"
#include "FileNode.h"
#include "ProjectTreeManager.h"
#include "LinePainter.h"
#include "LineCleaner.h"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <texteditor/itexteditor.h>
#include <texteditor/basetexteditor.h>

#include <QPlainTextEdit>
#include <QScrollBar>

#include <QDebug>

Visualizer::Visualizer(ProjectTreeManager *projectTreeManager, QAction *renderAction, QObject *parent) :
    QObject(parent),
    projectTreeManager(projectTreeManager),
    markManager(new MarkManager(this)),
    renderAction(renderAction)
{
    using namespace Core;
    EditorManager *editorManager = EditorManager::instance();

    connect(editorManager,SIGNAL(currentEditorChanged(Core::IEditor*)),SLOT(renderCurrentCoverage()),Qt::QueuedConnection);
    connect(editorManager,SIGNAL(currentEditorChanged(Core::IEditor*)),SLOT(bindEditorToPainting(Core::IEditor*)));

    connect(renderAction,SIGNAL(triggered(bool)),SLOT(repaintMarks(bool)));
    connect(renderAction,SIGNAL(triggered(bool)),SLOT(renderCoverage()));
}

void Visualizer::refreshMarks()
{
    markManager->removeAllMarks();

    Node *rootNode = projectTreeManager->getRootNode();
    QList<Node *> leafs = rootNode->getLeafs();

    foreach (Node *leaf, leafs) {
        if (!leaf->isFileNode())
            continue;

        FileNode *fileNode = static_cast<FileNode *>(leaf);

        QString fileName = fileNode->getFullAbsoluteName().replace(QRegExp(QLatin1String("(Sources|Headers)/")), QLatin1String(""));
        const LineHitList &lineHitList = fileNode->getLineHitList();
        const BranchCoverageList &branchCoverageList = fileNode->getBranchCoverageList();

        foreach (const LineHit &lineHit, lineHitList) {
            markManager->addMark(fileName, lineHit.pos)->setType((lineHit.hit != 0) ? Mark::Good : Mark::NotHit);
        }

        foreach (const BranchCoverage &branchCoverage, branchCoverageList) {
            if (branchCoverage.count != branchCoverage.covered) {
                markManager->addMark(fileName, branchCoverage.pos)->setType(
                            branchCoverage.covered ? Mark::NotBranchCovered : Mark::NotHit);
            }
        }
    }
}

static QPlainTextEdit *getTextEdit(Core::IEditor *editor)
{
    if (!editor)
        return 0;

    return qobject_cast<QPlainTextEdit *>(editor->widget());
}

void Visualizer::renderCoverage()
{
    using namespace Core;
    using namespace TextEditor;

    EditorManager *editorManager = EditorManager::instance();

    const QList<IEditor *> &editors = editorManager->openedEditors();
    foreach (IEditor *editor, editors) {
        if (renderAction->isChecked()) {
            renderCoverage(editor);
        } else {
            clearCoverage(editor);
        }
    }
}

void Visualizer::renderCurrentCoverage()
{
    using namespace Core;
    using namespace TextEditor;

    EditorManager *editorManager = EditorManager::instance();

    if (renderAction->isChecked()) {
        renderCoverage(editorManager->currentEditor());
    } else {
        clearCoverage(editorManager->currentEditor());
    }
}

void Visualizer::renderCoverage(Core::IEditor *editor) const
{
    QPlainTextEdit *plainTextEdit = getTextEdit(editor);
     if (!plainTextEdit)
         return;

    LinePainter painter(plainTextEdit);
    painter.render(getLineCoverage(editor));
}

void Visualizer::clearCoverage(Core::IEditor *editor) const
{
    QPlainTextEdit *plainTextEdit = getTextEdit(editor);
     if (!plainTextEdit)
         return;

    LineCleaner cleaner(plainTextEdit);
    cleaner.render(getLineCoverage(editor));
}

void Visualizer::bindEditorToPainting(Core::IEditor *editor)
{
    if (!editor)
        return;

    if (QPlainTextEdit *plainTextEdit = qobject_cast<QPlainTextEdit *>(editor->widget())) {
        connect(plainTextEdit,SIGNAL(cursorPositionChanged()),SLOT(renderCurrentCoverage()), Qt::UniqueConnection);
        connect(plainTextEdit->verticalScrollBar(),SIGNAL(valueChanged(int)),SLOT(renderCurrentCoverage()),Qt::UniqueConnection);
    }
}

void Visualizer::repaintMarks(bool isRender)
{
    if (!isRender) {
        markManager->removeAllMarks();
        markManager->addMark(QLatin1String(""), 0);
    } else {
        refreshMarks();
    }
}

TextEditor::ITextEditor *Visualizer::currentTextEditor() const
{
    Core::EditorManager *em = Core::EditorManager::instance();
    Core::IEditor *currEditor = em->currentEditor();
    if (!currEditor)
        return 0;
    return qobject_cast<TextEditor::ITextEditor *>(currEditor);
}

QMap<int, Mark *> Visualizer::getLineCoverage(Core::IEditor *editor) const
{
    if (editor)
        return markManager->getMarks(editor->document()->fileName());
    else
        return QMap<int, Mark *>();
}
