#include "mainwindow.h"

#include "graph.h"
#include "graphcontrolwidget.h"
#include "graphvisualizer.h"

#include <QDebug>
#include <QHBoxLayout>

#include <chrono>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Fake News Simulator");

    QWidget* centralWidget = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(centralWidget);

    GraphVisualizer* graphVisualizer = new GraphVisualizer;
    GraphControlWidget* graphControlWidget = new GraphControlWidget;

    layout->addWidget(graphVisualizer);
    layout->addWidget(graphControlWidget);

    setCentralWidget(centralWidget);

    connect(graphControlWidget, &GraphControlWidget::createGraphRequested,
        [this, graphVisualizer]() {
            std::shared_ptr<Graph> graph = std::make_shared<Graph>();
            _simulator.setGraph(graph);
            graphVisualizer->setGraph(graph);
        });
    connect(graphControlWidget, &GraphControlWidget::runSimulationRequested,
        [this]() {
            auto startTime = std::chrono::high_resolution_clock::now();

            _simulator.simulate();

            auto endTime = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed = endTime - startTime;
            qDebug() << "Elapsed Time (seconds): " << elapsed.count();
        });

    setFixedSize(sizeHint());
}
