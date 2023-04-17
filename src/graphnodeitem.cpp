#include "graphnodeitem.h"

#include "node.h"

#include <QCoreApplication>
#include <QPainter>

GraphNodeItem::GraphNodeItem(Node& node, const QPointF& center, qreal radius, QGraphicsItem* parent)
    : QGraphicsEllipseItem(center.x() - radius, center.y() - radius, 2 * radius, 2 * radius, parent)
    , _node(node)
{
    auto onScoreUpdate = [this](int s) {
        QColor color = QColor(0x48C9B);
        if (s > 0.5) {
            color = QColor(0xF4D03F);
        } else if (s > -0.5) {
            color = QColor(0x4A90E2);
        }
        setBrush(color);
        update();
    };
    node.setOnScoreUpdate(onScoreUpdate);

    setBrush(stateColor());
    setPen(QColor(Qt::black));
}

void GraphNodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    constexpr int fontSize = 6;

    QGraphicsEllipseItem::paint(painter, option, widget);

    painter->setPen(QPen(Qt::white));
    QFont font = painter->font();
    font.setPointSize(fontSize);
    painter->setFont(font);
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(_node.id()));
}

QColor GraphNodeItem::stateColor() const
{
    switch (_node.state()) {
    case REGULAR:
        return 0x0B3D91;
        break;
    case INFLUENCER:
        return 0xF1C40F;
        break;
    case BOT:
        return 0x7D3C98;
        break;
    case BELIEVER:
        return 0xC0392B;
        break;
    case FACT_CHECKER:
        return 0x228B22;
        break;
    default:
        return 0xD3D3D3;
        break;
    }
}
