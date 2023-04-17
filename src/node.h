#pragma once

#include <fstream>
#include <functional>
#include <queue>
#include <string>
#include <utility>
#include <vector>

enum NodeState { REGULAR, INFLUENCER, BOT, BELIEVER, FACT_CHECKER };

struct Edge {
    size_t dest;
    double weight;

    Edge(size_t d, double w)
        : dest(d)
        , weight(w)
    {
    }
};

struct Message {
    int type;
    double weight;

    Message(int t, double w)
        : type(t)
        , weight(w)
    {
    }
};

class Node {
public:
    Node(size_t i, NodeState s);

    size_t id() const { return _id; }
    NodeState state() const { return _state; }
    const std::vector<double>& coordinates() const { return _coordinates; }
    int score() const { return _score; }
    void setScore(int s, double timestamp = -1);
    void setOnScoreUpdate(const std::function<void(int)>& onScoreUpdate) { _onScoreUpdate = onScoreUpdate; }
    double reshareRate() const { return _reshareRate; }
    const std::vector<Edge>& outgoingEdges() const { return _outgoingEdges; }

    void initializePatientZero();
    void addMessage(int score, double weight);
    void update(double engagementFactor, double timestamp);
    void connect(size_t to, double weight);

    double spatialDistance(const Node& other) const;
    double attributeDistance(const Node& other) const;

    static void openLogFile(const std::string& filename);
    static void closeLogFile();

private:
    static double generateRecoverRate();
    static double generateVulnerability();
    std::vector<double> generateInterestAttributes() const;
    static double generateReshareRate();
    void tryRecover(double timestamp);
    static double euclideanDistance(const std::vector<double>& v1, const std::vector<double>& v2);

    size_t _id;
    NodeState _state;
    std::vector<double> _coordinates;
    double _recoverRate;
    double _vulnerability;
    int _score;
    std::function<void(int)> _onScoreUpdate;
    std::vector<double> _interestAttributes;
    double _reshareRate;
    std::vector<Edge> _outgoingEdges;
    std::queue<Message> _messages;
    static std::ofstream _logFile;
};
