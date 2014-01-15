#pragma once

#include "Node.h"
#include "FileCoverageData.h"

class FileNode : public Node
{
    LineHitList lineHitList;
    BranchCoverageList branchCoverageList;
public:
    FileNode(const QString &name, Node *parent);

    QVariant getBranchData() const;
    QIcon getIcon() const;
    bool isFileNode() const;

    int getLineCount() const;
    int getLineHitCount() const;

    LineHitList getLineHitList() const;
    void setLineHitList(const LineHitList &lineHitList);

    BranchCoverageList getBranchCoverageList() const;
    void setBranchCoverageList(const BranchCoverageList &branchCoverage);
};

