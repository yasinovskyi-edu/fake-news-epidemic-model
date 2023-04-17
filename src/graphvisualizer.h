#pragma once

#include <QGraphicsView>

#include <memory>

class Graph;
class Node;

class GraphVisualizer : public QGraphicsView {
public:
    explicit GraphVisualizer(QWidget* parent = nullptr);

    void setGraph(std::shared_ptr<Graph> graph);

protected:
    void wheelEvent(QWheelEvent* event) override;

private:
    void drawNodes();
    void drawEdges();

    static QPointF scaledCoordinates(const Node& node);
    static QPointF intersectionPoint(const QPointF& center, double radius, QLineF line);

    std::shared_ptr<Graph> _graph;
};
