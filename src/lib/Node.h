#pragma once

#include <QString>
#include <QList>
#include <QVariant>

class Node
{
    QString name;
    Node *parent;
    QList<Node *> children;
public:
    Node(const QString &name = QString(), Node *parent = 0);
    virtual ~Node();

    virtual QVariant getLineData() const;
    virtual QVariant getBranchData() const;
    virtual QIcon getIcon() const;
    virtual bool isFileNode() const;

    virtual int getLineCount() const;
    virtual int getLineHitCount() const;

    virtual QString getFullAbsoluteName() const;
    QString getFullName() const;
    QString getName() const;

    Node *getParent() const;

    void addChild(Node *child);
    void removeChild(Node *child);
    bool hasChild(const QString &name) const;
    Node *getChild(const QString &name) const;
    bool hasChildren() const;
    QList<Node *> getChildren() const;


    QList<Node *> findLeafs(const QString &name) const;

    QList<Node *> getLeafs() const;
};
