#include "CleanExecutor.h"

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/project.h>
#include <projectexplorer/target.h>
#include <projectexplorer/projectnodes.h>
#include <projectexplorer/buildconfiguration.h>

#include <QDebug>

CleanExecutor::CleanExecutor(QObject *parent) :
    ProcessExecutor(parent)
{
}

void CleanExecutor::execute()
{
    using namespace ProjectExplorer;

    ProjectExplorerPlugin *projectExplorerPlugin = ProjectExplorerPlugin::instance();
    Project *project = projectExplorerPlugin->currentProject();
    BuildConfiguration *buildConf = project->activeTarget()->activeBuildConfiguration();

    const QString &objectFilesDir = buildConf->buildDirectory().toString();

    const QString program = QLatin1String("lcov");
    const QStringList arguments = {
        QLatin1String("-z"),
        QLatin1String("-d"),
        objectFilesDir
    };

    process->start(program, arguments);
}
