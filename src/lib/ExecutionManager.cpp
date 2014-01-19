#include "ExecutionManager.h"

#include "Executor.h"
#include "ActionManager.h"

#include <QAction>
#include <coreplugin/messagemanager.h>

ExecutionManager::ExecutionManager(ActionManager *actionManager, QObject *parent) :
    QObject(parent),
    actionManager(actionManager),
    isExecuting(false)
{    
    connect(actionManager->getRunAction(),SIGNAL(triggered()),SLOT(startExecuting()));
    connect(actionManager->getWaitAction(),SIGNAL(triggered()),SLOT(stopExecuting()));
}

void ExecutionManager::addExecutor(Executor *executor)
{    
    connect(executor,SIGNAL(finished()),SLOT(executeNext()));
    connect(executor,SIGNAL(error(QString)),SLOT(error(QString)));

    executors << executor;
}

void ExecutionManager::startExecuting()
{
    if (!isExecuting) {
        executorsQueue.clear();
        foreach (Executor *executor, executors)
            executorsQueue.enqueue(executor);

        isExecuting = true;
        executeNext();
    }
}

void ExecutionManager::executeNext()
{
    if (isExecuting) {
        if (!executorsQueue.isEmpty()) {
            Executor *executor = executorsQueue.dequeue();
            executor->execute();
        } else {
            stopExecuting();
        }
    }
}

void ExecutionManager::stopExecuting()
{
    if (isExecuting) {
        isExecuting = false;
        actionManager->getWaitAction()->trigger();
    }
}

void ExecutionManager::error(const QString &err)
{
    if (!err.isEmpty())
    {
        using namespace Core;
        MessageManager::instance()->printToOutputPane(err, MessageManager::Flash);
    }
    stopExecuting();
}
