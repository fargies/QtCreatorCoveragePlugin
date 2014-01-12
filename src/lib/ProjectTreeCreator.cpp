#include "ProjectTreeCreator.h"

#include "Node.h"
#include "TreeCreator.h"
#include <QDebug>

static const char *SPLIT_SEPARATOR = "/";
static const char *OR_SYMBOL = "|";
static const int SPECIAL_DIRECTORY_POS = 0;

ProjectTreeCreator::ProjectTreeCreator(const QStringList &fileNames, Node *rootNode) :
    rootNode(rootNode ? rootNode : new Node()),
    fileNames(fileNames)
{
}

void ProjectTreeCreator::execute()
{
    const QStringList &fileNames = getFileNamesWithProjectSpecialDirectory();

    TreeCreator treeCreator(fileNames, rootNode);
    treeCreator.execute();
}

Node *ProjectTreeCreator::getRootNode() const
{
    return rootNode;
}

Node *ProjectTreeCreator::getLeafNodeFromFullName(const QString &fullName, const QString &projectShortName) const
{
    const QString &fileNameWithSpecialDirectory = getFileNameWithProjectSpecialDirectory(fullName);
    const QList<Node *> &leafs = rootNode->findLeafs(fileNameWithSpecialDirectory.split(QLatin1Char('/')).last());

    foreach (Node *leaf, leafs)
        if (leaf->getFullName() == projectShortName + QLatin1String(SPLIT_SEPARATOR) + fileNameWithSpecialDirectory)
            return leaf;   

    return 0;
}

QString ProjectTreeCreator::getFileNameWithProjectSpecialDirectory(const QString &fileName) const
{
    const QMultiHash<QString, QString> &specialDirectory = SpecialDirectoryTable();

    foreach (const QString &directoryName, specialDirectory.keys()) {
        const QStringList &extensions = specialDirectory.values(directoryName);
        const QString &filterPattern = QString(QLatin1String(".*\\.(%1)")).arg(extensions.join(QLatin1String(OR_SYMBOL)));
        const QRegExp filterRegExp(filterPattern);
        if (fileName.contains(filterRegExp)) {
            QStringList nodeNames = fileName.split(QLatin1String(SPLIT_SEPARATOR));
            nodeNames.insert(SPECIAL_DIRECTORY_POS, directoryName);
            return nodeNames.join(QLatin1String(SPLIT_SEPARATOR));
        }
    }

    return QString();
}

QStringList ProjectTreeCreator::getFileNamesWithProjectSpecialDirectory() const
{
    QStringList fileNames;
    foreach (const QString &fileName, this->fileNames)
        fileNames.append(getFileNameWithProjectSpecialDirectory(fileName));

    return fileNames;
}

QMultiHash<QString, QString> ProjectTreeCreator::SpecialDirectoryTable()
{
    static QMultiHash<QString, QString> specialDirectoryTable;

    if (specialDirectoryTable.isEmpty()) {
        specialDirectoryTable.insert(QLatin1String("Headers"), QLatin1String("h"));
        specialDirectoryTable.insert(QLatin1String("Sources"), QLatin1String("cpp"));
    }

    return specialDirectoryTable;
}
