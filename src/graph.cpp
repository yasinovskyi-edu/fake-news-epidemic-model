#include "graph.h"

#include "graphsettings.h"
#include "randomgenerators.h"

#include <fstream>

constexpr double CONNECTION_PROB = 0.5;
constexpr double BOT_CONNECTION_PROB = 0.02;
constexpr double BOT_WEIGHT = 0.1;

Graph::Graph()
{
    const GraphSettings& settings = GraphSettings::instance();
    _nodes.reserve(settings.numRegular + settings.numInfluencers + settings.numBots);

    generateRegularNodes();
    generateInfluencerNodes();
    generateBotNodes();
    generateFactCheckerNodes();

//    std::vector<size_t> outgoingEdgeCounts(_nodes.size(), 0);
//    std::vector<size_t> ingoingEdgeCounts(_nodes.size(), 0);

//    for (const Node& node : _nodes) {
//        outgoingEdgeCounts[node.id()] = node.outgoingEdges().size();
//        for (const Edge& edge : node.outgoingEdges()) {
//            ++ingoingEdgeCounts[edge.dest];
//        }
//    }

//    std::ofstream outfile("edge_distribution.csv");
//    outfile << "NodeID,State,OutgoingEdges,IngoingEdges\n";
//    for (size_t i = 0; i < _nodes.size(); ++i) {
//        outfile << i << ","
//                << stateToString(_nodes[i].state()) << ","
//                << outgoingEdgeCounts[i] << ","
//                << ingoingEdgeCounts[i] << "\n";
//    }
//    outfile.close();
}

void Graph::generateRegularNodes()
{
    const GraphSettings& settings = GraphSettings::instance();

    for (size_t i = 0; i < settings.numRegular; ++i) {
        _nodes.emplace_back(i, REGULAR);

        for (size_t j = 0; j < i; ++j) {
            // TODO: Rewrite the functionality of connecting nodes based on probability. It's
            // messed up to correspond to the original Python-based model
            double p = randomUniform(0, 1);
            double spDist = _nodes[i].spatialDistance(_nodes[j]);

            if (p < CONNECTION_PROB && spDist < settings.spatialThreshold) {
                double weight = 1 - spDist;
                p = randomUniform(0, 1);

                if (p < CONNECTION_PROB) {
                    double attrDist = _nodes[i].attributeDistance(_nodes[j]);
                    double attrDistWt = 1 - attrDist;
                    weight = std::min(weight, attrDistWt);
                }

                connectNode(i, j, weight);
                connectNode(j, i, weight);
            } else {
                p = randomUniform(0, 1);
                double attrDist = _nodes[i].attributeDistance(_nodes[j]);

                if (p < CONNECTION_PROB && attrDist < settings.attrThreshold) {
                    double weight = 1 - attrDist;
                    connectNode(i, j, weight);
                    connectNode(j, i, weight);
                }
            }
        }
    }
}

void Graph::generateInfluencerNodes()
{
    generateHubNodes(INFLUENCER);
}

void Graph::generateBotNodes()
{
    const GraphSettings& settings = GraphSettings::instance();

    for (size_t i = settings.numRegular + settings.numInfluencers;
         i < settings.numRegular + settings.numInfluencers + settings.numBots;
         ++i) {
        _nodes.emplace_back(i, BOT);

        for (size_t j = 0; j < settings.numRegular + settings.numInfluencers; ++j) {
            double p = randomUniform(0, 1);

            if (p < BOT_CONNECTION_PROB) {
                connectNode(i, j, BOT_WEIGHT);
                connectNode(j, i, BOT_WEIGHT);
            }
        }
    }
}

void Graph::generateFactCheckerNodes()
{
    generateHubNodes(FACT_CHECKER);
}

void Graph::generateHubNodes(NodeState state)
{
    const GraphSettings& settings = GraphSettings::instance();
    size_t i;
    size_t endIdx;

    if (state == INFLUENCER) {
        i = settings.numRegular;
        endIdx = i + settings.numInfluencers;
    } else {
        i = settings.numRegular + settings.numInfluencers + settings.numBots;
        endIdx = i + settings.numFactCheckers;
    }

    for ( ; i < endIdx; ++i) {
        _nodes.emplace_back(i, state);

        for (size_t j = 0; j < i; ++j) {
            double spDist = _nodes[i].spatialDistance(_nodes[j]);
            double attrDist = _nodes[i].attributeDistance(_nodes[j]);

            if (spDist < 2 * settings.spatialThreshold) {
                double weight = 1 - spDist;

                if (attrDist < 2 * settings.attrThreshold) {
                    double attrDistWt = 1 - attrDist;
                    // TODO: Replace with min instead of mean
                    weight = (weight + attrDistWt) / 2;
                }

                connectNode(i, j, weight);

                if (spDist < settings.spatialThreshold) {
                    weight = 1 - spDist;

                    if (attrDist < settings.attrThreshold) {
                        double attrDistWt = 1 - attrDist;
                        weight = (weight + attrDistWt) / 2;
                    }

                    connectNode(j, i, weight);
                } else if (attrDist < settings.attrThreshold) {
                    weight = 1 - attrDist;
                    connectNode(j, i, weight);
                }
            } else if (attrDist < 2 * settings.attrThreshold) {
                double weight = 1 - attrDist;
                connectNode(i, j, weight);

                if (attrDist < settings.attrThreshold) {
                    connectNode(j, i, weight);
                }
            }
        }
    }
}

void Graph::connectNode(size_t from, size_t to, double weight) { _nodes[from].connect(to, weight); }

std::string Graph::stateToString(NodeState state)
{
    switch (state) {
    case REGULAR: return "Regular";
    case BELIEVER: return "Believer";
    case INFLUENCER: return "Influencer";
    case BOT: return "Bot";
    case FACT_CHECKER: return "Fact-Checker";
    default: return "";
    }
}
