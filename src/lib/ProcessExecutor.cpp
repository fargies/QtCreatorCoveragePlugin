#include "ProcessExecutor.h"

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/buildconfiguration.h>

#include <QDebug>

ProcessExecutor::ProcessExecutor(QObject *parent) :
    Executor(parent),
    process(new QProcess(this))
{
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),SLOT(handleCoverageResults(int,QProcess::ExitStatus)));
    connect(process,SIGNAL(error(QProcess::ProcessError)),SLOT(handleError(QProcess::ProcessError)));
}

void ProcessExecutor::execute()
{
    using namespace ProjectExplorer;

    ProjectExplorerPlugin *projectExplorerPlugin = ProjectExplorerPlugin::instance();
    Project *project = projectExplorerPlugin->currentProject();
    BuildConfiguration *buildConf = project->activeTarget()->activeBuildConfiguration();

    const QString &objectFilesDir = buildConf->buildDirectory().toString();

    const QString &rootDir = project->projectDirectory();
    QDir dir(rootDir);
    dir.mkdir(QLatin1String("./coverage"));
    const QString outputFileName = rootDir + QLatin1Char('/') + QLatin1String("coverage/result.info");

    const QString program = QLatin1String("lcov");
    const QStringList arguments = {
        QLatin1String("-d"),
        objectFilesDir,
        QLatin1String("-c"),
        QLatin1String("-o"),
        outputFileName
    };

    process->start(program, arguments);
}

void ProcessExecutor::handleCoverageResults(int code, QProcess::ExitStatus exitStatus)
{
    if (code == 0 && exitStatus == QProcess::NormalExit)
        emit finished();
    else
        emit error(QLatin1String("lcov failure: ") +
                   QLatin1String(process->readAllStandardError()));
}

void ProcessExecutor::handleError(QProcess::ProcessError)
{
    emit error(QLatin1String("lcov failure: ") + process->errorString());
}
