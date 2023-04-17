#include "graphvisualizer.h"

#include "graph.h"
#include "graphedgeitem.h"
#include "graphnodeitem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <QWheelEvent>
#include <QtMath>

constexpr double SCALING_FACTOR = 1000.0;
constexpr double MARGIN = 20.0;
constexpr double NODE_RADIUS = 10.0;

GraphVisualizer::GraphVisualizer(QWidget* parent)
    : QGraphicsView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setFixedSize(1040, 1040);
}

void GraphVisualizer::setGraph(std::shared_ptr<Graph> graph)
{
    delete scene();
    setScene(new QGraphicsScene(this));

    _graph = std::move(graph);

    drawEdges();
    drawNodes();

//    QPixmap pixmap(scene()->width(), scene()->height());
//    pixmap.fill(Qt::transparent);

//    QPainter painter(&pixmap);
//    scene()->render(&painter);

//    pixmap.save("network.png", "PNG");
}

void GraphVisualizer::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        double zoomFactor = (event->angleDelta().y() > 0) ? 1.1 : 0.9;
        double currentZoomFactor = transform().m11();

        if ((currentZoomFactor < 0.1 && zoomFactor < 1) || (currentZoomFactor > 10 && zoomFactor > 1)) {
            return;
        }

        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        scale(zoomFactor, zoomFactor);
    } else {
        QGraphicsView::wheelEvent(event);
    }
}

void GraphVisualizer::drawNodes()
{
    for (auto& node : _graph->nodes()) {
        QPointF center = scaledCoordinates(node);

        GraphNodeItem* nodeItem = new GraphNodeItem(node, center, NODE_RADIUS);
        scene()->addItem(nodeItem);
    }
}

void GraphVisualizer::drawEdges()
{
    for (const auto& srcNode : _graph->nodes()) {
        for (const auto& edge : srcNode.outgoingEdges()) {
            const Node& destNode = _graph->nodes()[edge.dest];

            QPointF srcCenter = scaledCoordinates(srcNode);
            QPointF destCenter = scaledCoordinates(destNode);
            QLineF line(srcCenter, destCenter);

            QPointF srcIntersection = intersectionPoint(srcCenter, NODE_RADIUS, line);
            QPointF destIntersection = intersectionPoint(destCenter, NODE_RADIUS, line);

            if (!srcIntersection.isNull() && !destIntersection.isNull()) {
                GraphEdgeItem* edgeItem = new GraphEdgeItem(QLineF(srcIntersection, destIntersection));
                scene()->addItem(edgeItem);
            }
        }
    }
}

QPointF GraphVisualizer::scaledCoordinates(const Node& node)
{
    double x = node.coordinates()[0] * SCALING_FACTOR + MARGIN;
    double y = node.coordinates()[1] * SCALING_FACTOR + MARGIN;
    return QPointF(x, y);
}

QPointF GraphVisualizer::intersectionPoint(const QPointF& center, double radius, QLineF line)
{
    if (line.x2() < line.x1()) {
        line.setPoints(line.p2(), line.p1());
    }

    double slope = line.dy() / line.dx();
    double yIntercept = line.y1() - slope * line.x1();

    double a = 1 + slope * slope;
    double b = 2 * (slope * (yIntercept - center.y()) - center.x());
    double c = center.x() * center.x() + (yIntercept - center.y()) * (yIntercept - center.y()) - radius * radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return {};
    }

    double x1 = (-b + qSqrt(discriminant)) / (2 * a);
    double y1 = slope * x1 + yIntercept;

    if (x1 >= line.x1() && x1 <= line.x2()) {
        return QPointF(x1, y1);
    }

    double x2 = (-b - qSqrt(discriminant)) / (2 * a);
    double y2 = slope * x2 + yIntercept;

    if (x2 >= line.x1() && x2 <= line.x2()) {
        return QPointF(x2, y2);
    }

    return {};
}
