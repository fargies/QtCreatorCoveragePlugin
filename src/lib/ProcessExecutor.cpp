#include "ProcessExecutor.h"

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/localapplicationrunconfiguration.h>

#include <QDebug>

ProcessExecutor::ProcessExecutor(QObject *parent) :
    Executor(parent),
    process(new QProcess(this))
{
    connect(process,SIGNAL(readyReadStandardOutput()),SLOT(readOutput()));
    connect(process,SIGNAL(readyReadStandardError()),SLOT(readError()));
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),SLOT(handleCoverageResults(int,QProcess::ExitStatus)));
}

void ProcessExecutor::execute()
{
    using namespace ProjectExplorer;

    ProjectExplorerPlugin *projectExplorerPlugin = ProjectExplorerPlugin::instance();
    Project *project = projectExplorerPlugin->startupProject();

    const QString &activeRunConfigurationDir = getRunConfigurationPath(project->activeTarget()->activeRunConfiguration());

    const QString &buildDir = activeRunConfigurationDir.mid(0, activeRunConfigurationDir.lastIndexOf(QLatin1Char('/')));
    const QString &objectFilesDir = getObjectFilesDir(buildDir);

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
        outputFileName,
        QLatin1String("-b"),
        buildDir
    };

    process->start(program, arguments);
}

void ProcessExecutor::readOutput()
{
}

void ProcessExecutor::readError()
{
}

void ProcessExecutor::handleCoverageResults(int code, QProcess::ExitStatus exitStatus)
{    
    if (code == 0 && exitStatus == QProcess::NormalExit)
        emit finished();
    else
        emit error();
}

//#TOTEST:
QString ProcessExecutor::getRunConfigurationPath(ProjectExplorer::RunConfiguration *activeRunConfiguration) const
{
    using namespace ProjectExplorer;
    LocalApplicationRunConfiguration *localConf =
        qobject_cast<LocalApplicationRunConfiguration *>(activeRunConfiguration);
    if (localConf)
        return localConf->workingDirectory();

    return QString();
}

//#TOTEST:
QString ProcessExecutor::getObjectFilesDir(const QString &buildDir) const
{
    QFile makeFile(buildDir + QLatin1String("/Makefile"));
    QTextStream out(&makeFile);

    if (makeFile.open(QIODevice::ReadOnly)) {
        while (!out.atEnd()) {
            const QString &line = out.readLine();

            QRegExp rx(QLatin1String("OBJECTS_DIR\\s*=\\s*([^\\s]*)$"));
            if (rx.indexIn(line) != -1) {
                const QString &objectsDir = rx.cap(1);
                return QString(QLatin1String("%1/%2")).arg(buildDir).arg(objectsDir);
            }
        }
    }

    return buildDir;
}
