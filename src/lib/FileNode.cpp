#include "FileNode.h"

#include "IconDecorator.h"

#include <coreplugin/fileiconprovider.h>

#include <QObject>
#include <QDebug>

FileNode::FileNode(const QString &name, Node *parent) :
    Node(name, parent)
{
}

QVariant FileNode::getLineData() const
{
    if (lineHitList.isEmpty())
        return QLatin1String("-");

    int linesCovered = 0;
    foreach (const LineHit &lineHit, lineHitList)
        if (lineHit.hit > 0)
            linesCovered++;

    double linesCoveredPercentage = linesCovered *100 / (double)lineHitList.size();
    return QObject::tr("%1%").arg(QString::number(linesCoveredPercentage, 'f', 1));
}

QVariant FileNode::getBranchData() const
{
    if (branchCoverageList.isEmpty())
        return QLatin1String("-");

    int branchesCovered = 0;
    int branches = 0;
    foreach (const BranchCoverage &branch, branchCoverageList)
    {
        branchesCovered += branch.covered;
        branches += branch.count;
    }

    if (branches == 0)
        return QLatin1String("-"); /* shouldn't happen */

    double branchCoveredPercentage = branchesCovered * 100 / (double) branches;
    return QObject::tr("%1%").arg(QString::number(branchCoveredPercentage, 'f', 1));
}

QIcon FileNode::getIcon() const
{
    const QString &fullName = getFullName();
    if (fullName.contains(QLatin1String("/Headers/")))
        return IconDecorator::getIconByName(QLatin1String("File.Headers"));
    if (fullName.contains(QLatin1String("/Sources/")))
        return IconDecorator::getIconByName(QLatin1String("File.Sources"));

    return Node::getIcon();
}

bool FileNode::isFileNode() const
{
    return true;
}

LineHitList FileNode::getLineHitList() const
{
    return lineHitList;
}

void FileNode::setLineHitList(const LineHitList &lineHitList)
{
    this->lineHitList = lineHitList;
}

BranchCoverageList FileNode::getBranchCoverageList() const
{
    return branchCoverageList;
}

void FileNode::setBranchCoverageList(const BranchCoverageList &branchCoverage)
{
    this->branchCoverageList = branchCoverage;
}
