#include "ProjectTreeCreator.h"

#include "Node.h"
#include "TreeCreator.h"

static const QLatin1Char SPLIT_SEPARATOR('/');
static const int SPECIAL_DIRECTORY_POS = 0;

ProjectTreeCreator::ProjectTreeCreator(const QStringList &fileNames, Node *rootNode) :
    rootNode(rootNode ? rootNode : new Node()),
    fileNames(fileNames)
{
}

void ProjectTreeCreator::execute()
{
    TreeCreator treeCreator(fileNames, rootNode);
    treeCreator.execute();
}

Node *ProjectTreeCreator::getRootNode() const
{
    return rootNode;
}

Node *ProjectTreeCreator::getLeafNodeFromFullName(const QString &fullName, const QString &projectShortName) const
{
    /* TODO: handle relative paths in info file here (remove ../.. from path and compare if matching part is unique) */
    const QList<Node *> &leafs = rootNode->findLeafs(fullName.split(SPLIT_SEPARATOR).last());

    foreach (Node *leaf, leafs)
        if (leaf->getFullName() == projectShortName + SPLIT_SEPARATOR + fullName)
            return leaf;

    return 0;
}
