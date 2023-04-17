#include "simulator.h"

#include "graph.h"
#include "graphsettings.h"
#include "randomgenerators.h"

#include <QCoreApplication>
#include <QMessageBox>

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

void Simulator::setGraph(std::shared_ptr<Graph> graph)
{
    _graph = std::move(graph);
}

void Simulator::simulate()
{
    Node::openLogFile("high_influencer.csv");

    size_t initialInfection = randint(0, _graph->nodes().size() - 1);
    _graph->nodes()[initialInfection].initializePatientZero();
    initializeEventsQueue(initialInfection);

    const GraphSettings& settings = GraphSettings::instance();
    double currentTime = 0;

    while (currentTime < settings.simTime) {
        const Event& event = _eventsQueue.top();
        currentTime = event.first;
        size_t i = event.second;
        _eventsQueue.pop();

        Node& node = _graph->nodes()[i];
        node.update(engagementFactor(currentTime), currentTime);

        const GraphSettings& settings = GraphSettings::instance();
        int score = node.score();

        if (score == 1 || (score == -1 && settings.recoveredNodesDebunk)) {
            double reshareRate = node.reshareRate();
            for (const Edge& edge : node.outgoingEdges()) {
                double p = randomUniform(0, 1);

                if (p < reshareRate && edge.dest != initialInfection) {
                    double weight = settings.isGraphWeighted ? edge.weight : 1;
                    Node& destNode = _graph->nodes()[edge.dest];
                    destNode.addMessage(score, weight);
                }
            }
        }

        double lambda = (node.state() == BOT) ? (1.0 / 4) : (1.0 / 16);
        double nextTime = currentTime + randomExponential(lambda);
        _eventsQueue.emplace(nextTime, i);

        QCoreApplication::processEvents();
    }

    Node::closeLogFile();
}

std::vector<size_t> Simulator::generateNodeIndices(size_t initialInfection) const
{
    std::vector<size_t> indices(_graph->nodes().size() - 1, 0);
    std::iota(indices.begin(), indices.begin() + initialInfection, 0);
    std::iota(indices.begin() + initialInfection, indices.end(), initialInfection + 1);
    std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device{}()));
    return indices;
}

void Simulator::initializeEventsQueue(size_t initialInfection)
{
    std::vector<size_t> indices = generateNodeIndices(initialInfection);
    _eventsQueue.emplace(0, initialInfection);
    for (size_t i : indices) {
        double nextTime = randomExponential(1.0 / 16);
        _eventsQueue.emplace(nextTime, i);
    }
}

double Simulator::engagementFactor(double currentTime)
{
    const GraphSettings& settings = GraphSettings::instance();
    return std::exp(-1 / (settings.simTime / 2) * currentTime);
}
