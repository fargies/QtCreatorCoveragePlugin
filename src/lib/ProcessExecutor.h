#pragma once

#include "Executor.h"

#include <QProcess>

namespace ProjectExplorer {
class ProjectNode;
class RunConfiguration;
}

class ProcessExecutor : public Executor
{
    Q_OBJECT
protected:
    QProcess *process;

public:
    explicit ProcessExecutor(QObject *parent = 0);

public Q_SLOTS:
    void execute();

private Q_SLOTS:
    void handleCoverageResults(int code,QProcess::ExitStatus exitStatus);
    void handleError(QProcess::ProcessError err);
};
