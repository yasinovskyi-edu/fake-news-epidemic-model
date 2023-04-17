#pragma once

#include <QGraphicsEllipseItem>

#include <memory>

class Node;

class GraphNodeItem : public QGraphicsEllipseItem {
public:
    GraphNodeItem(Node& node, const QPointF& center, qreal radius, QGraphicsItem* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    QColor stateColor() const;

    const Node& _node;
};
