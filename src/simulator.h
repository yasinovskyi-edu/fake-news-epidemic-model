#pragma once

#include <memory>
#include <queue>
#include <utility>
#include <vector>

class Graph;

class Simulator {
public:
    void setGraph(std::shared_ptr<Graph> graph);

    void simulate();

private:
    using Event = std::pair<double, size_t>;

    struct EventComparator {
        bool operator()(const Event& e1, const Event& e2) { return e1.first > e2.first; }
    };

    std::vector<size_t> generateNodeIndices(size_t initialInfection) const;
    void initializeEventsQueue(size_t initialInfection);
    double engagementFactor(double currentTime);

    std::shared_ptr<Graph> _graph;
    std::priority_queue<Event, std::vector<Event>, EventComparator> _eventsQueue;
};
