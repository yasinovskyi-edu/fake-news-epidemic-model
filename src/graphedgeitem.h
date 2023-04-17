#pragma once

#include <QGraphicsLineItem>

class Node;

class GraphEdgeItem : public QGraphicsLineItem {
public:
    GraphEdgeItem(const QLineF& line, QGraphicsItem* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    void drawArrow(QPainter* painter) const;
};
