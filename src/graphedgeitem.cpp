#include "graphedgeitem.h"

#include <QPainter>
#include <QtMath>

GraphEdgeItem::GraphEdgeItem(const QLineF& line, QGraphicsItem* parent)
    : QGraphicsLineItem(line, parent)
{
}

void GraphEdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QGraphicsLineItem::paint(painter, option, widget);

    drawArrow(painter);
}

void GraphEdgeItem::drawArrow(QPainter* painter) const
{
    constexpr double arrowHeadSize = 10.0;
    double angle = std::atan2(-line().dy(), line().dx());

    QPointF arrowP1 = line().p2() + QPointF(sin(angle + M_PI + M_PI / 3) * arrowHeadSize,
                                            cos(angle + M_PI + M_PI / 3) * arrowHeadSize);
    QPointF arrowP2 = line().p2() + QPointF(sin(angle - M_PI / 3) * arrowHeadSize,
                                            cos(angle - M_PI / 3) * arrowHeadSize);

    QPolygonF arrowHead;
    arrowHead << line().p2() << arrowP1 << arrowP2;

    painter->setBrush(Qt::gray);
    painter->drawPolygon(arrowHead);
}
