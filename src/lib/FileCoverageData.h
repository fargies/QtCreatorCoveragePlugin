#pragma once

#include <QString>
#include <QList>

struct LineHit {
    LineHit() : pos(0), hit(0) {}
    LineHit(int pos, int hit) : pos(pos), hit(hit) {}

    int pos;
    int hit;

    inline bool operator ==(const LineHit &other) const {
        return pos == other.pos && hit == other.hit;
    }
};

typedef QList<LineHit> LineHitList;

struct BranchCoverage {
    BranchCoverage() : pos(0), count(0), covered(0) {}
    BranchCoverage(int pos, int count, int covered) :
        pos(pos), count(count), covered(covered) {}

    int pos;
    int count;
    int covered;

    inline bool operator ==(const BranchCoverage &other) const {
        return pos == other.pos && count == other.count
                && covered == other.covered;
    }
};

typedef QList<BranchCoverage> BranchCoverageList;

struct FileCoverageData
{
    FileCoverageData() {}
    FileCoverageData(const QString &fileName) : fileName(fileName) {}
    FileCoverageData(const QString &fileName, const LineHitList &lineHitList) : fileName(fileName), lineHitList(lineHitList) {}

    bool isValid() const {
        return !fileName.isEmpty();
    }

    QString fileName;
    LineHitList lineHitList;
    BranchCoverageList branchCoverageList;
};

typedef QList<FileCoverageData> FileCoverageDataList;
