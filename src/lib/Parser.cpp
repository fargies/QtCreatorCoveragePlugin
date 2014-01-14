#include "Parser.h"

#include <QDebug>

static const int INDEX_NOT_FOUND = -1;

Parser::Parser(const QStringList &fileNames, const QString &content) :
    fileNames(fileNames),
    content(content)
{
}

void Parser::execute()
{
    foreach (const QString fileName, fileNames) {
        const QStringList &fileCoverageDataContent = getFileCoverageDataContentByName(fileName);
        const FileCoverageData &fileCoverageData = parseFileCoverageDataContent(fileName, fileCoverageDataContent);
        if (fileCoverageData.isValid())
            fileCoverageDataList.append(fileCoverageData);
    }
}

FileCoverageDataList Parser::getFileCoverageDataList() const
{
    return fileCoverageDataList;
}

QStringList Parser::getFileCoverageDataContentByName(const QString &fileName) const
{
    QStringList ret;
    QString beginPattern = QString(QLatin1String("SF:%1")).arg(fileName);

    /* info files might contain more than one section for a given file */
    int idx = 0;
    do {
        int beginIndex = content.indexOf(beginPattern, idx);
        int endIndex = content.indexOf(QLatin1String("end_of_record"), beginIndex);

        if (beginIndex != INDEX_NOT_FOUND && endIndex != INDEX_NOT_FOUND)
            ret << content.mid(beginIndex, endIndex - beginIndex).split(QLatin1Char('\n'));
        idx = endIndex;
    } while (idx != INDEX_NOT_FOUND);

    return ret;
}

FileCoverageData Parser::parseFileCoverageDataContent(const QString &fileName, const QStringList &fileCoverageDataContent) const
{
    if (fileCoverageDataContent.isEmpty())
        return FileCoverageData();

    FileCoverageData fileCoverageData(fileName);
    parseCoverageData(fileCoverageData, fileCoverageDataContent);

    return fileCoverageData;
}

void Parser::parseCoverageData(FileCoverageData &data, const QStringList &fileCoverageDataStringList) const
{
    const QRegExp rxLh(QLatin1String("^DA:(\\d+),(\\d+)"));
    const QRegExp rxBc(QLatin1String("^BRDA:(\\d+),(\\d+),(\\d+),(\\d+|-)"));
    QMap<int, QSet<int> > bCovBranchs;
    QMap<int, QSet<int> > bCovCovered; /* a branch can be processed several
                                          times, thus we have to use a set to
                                          count covered branches */
    QMap<int, LineHit> lHitMap;

    foreach (const QString &fileCoverageDataString, fileCoverageDataStringList) {
        if (fileCoverageDataString.startsWith(QLatin1String("DA:"))) {
            int pos = rxLh.indexIn(fileCoverageDataString);
            if (pos > -1)
                lHitMap[rxLh.cap(1).toInt()].hit += rxLh.cap(2).toInt();
        }
        else if (fileCoverageDataString.startsWith(QLatin1String("BRDA:"))) {
            int pos = rxBc.indexIn(fileCoverageDataString);
            if (pos > -1) {
                int line = rxBc.cap(1).toInt();
                int branch = rxBc.cap(3).toInt();

                bCovBranchs[line].insert(branch);

                const QString &hit = rxBc.cap(4);
                if (hit != QLatin1String("-") &&
                        hit != QLatin1String("0"))
                    bCovCovered[line].insert(branch);
            }
        }
    }

    data.branchCoverageList.clear();
    QMap<int, QSet<int> >::const_iterator bCovIt(bCovBranchs.constBegin());
    for (; bCovIt != bCovBranchs.constEnd(); ++bCovIt)
        data.branchCoverageList.append(BranchCoverage(bCovIt.key(),
                                                      bCovIt.value().size(),
                                                      bCovCovered.value(bCovIt.key()).size()));

    QMap<int, LineHit>::iterator lHitIt(lHitMap.begin());
    for (; lHitIt != lHitMap.end(); ++lHitIt)
        lHitIt.value().pos = lHitIt.key();

    data.lineHitList = lHitMap.values();
}
