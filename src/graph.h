#pragma once

#include "node.h"

#include <string>
#include <vector>

class Graph {
public:
    Graph();

    std::vector<Node>& nodes() { return _nodes; }
    const std::vector<Node>& nodes() const { return _nodes; }

private:
    void generateRegularNodes();
    void generateInfluencerNodes();
    void generateBotNodes();
    void generateFactCheckerNodes();
    void generateHubNodes(NodeState state);
    void connectNode(size_t from, size_t to, double weight);
    static std::string stateToString(NodeState state);

    std::vector<Node> _nodes;
};
