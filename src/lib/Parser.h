#pragma once

#include "FileCoverageData.h"

#include <QString>
#include <QStringList>

class Parser
{
    const QStringList fileNames;
    const QString content;
    FileCoverageDataList fileCoverageDataList;
public:
    Parser(const QStringList &fileNames, const QString &content);

    void execute();

    FileCoverageDataList getFileCoverageDataList() const;

private:
    QStringList getFileCoverageDataContentByName(const QString &fileName) const;
    FileCoverageData parseFileCoverageDataContent(const QString &fileName, const QStringList &fileCoverageDataContent) const;
    void parseCoverageData(FileCoverageData &data, const QStringList &fileCoverageDataStringList) const;
};
