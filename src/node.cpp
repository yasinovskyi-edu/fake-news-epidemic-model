#include "node.h"

#include "graphsettings.h"
#include "randomgenerators.h"

#include <algorithm>
#include <cmath>
#include <numeric>

Node::Node(size_t id, NodeState state)
    : _id(id)
    , _state(state)
    , _coordinates(2)
    , _recoverRate(generateRecoverRate())
    , _vulnerability(generateVulnerability())
    , _score(0)
    , _interestAttributes(generateInterestAttributes())
{
    _coordinates[0] = randomUniform(0, 1);
    _coordinates[1] = randomUniform(0, 1);

    // TODO: Clean up
    const GraphSettings& settings = GraphSettings::instance();
    _reshareRate = (state == BOT) ? 1 : randomNormal(settings.reshareRateMean, settings.reshareRateStd, 0, 1);

    if (state == FACT_CHECKER) {
        _recoverRate = 1;
        _vulnerability = 0;
    }
}

void Node::setScore(int s, double timestamp)
{
    _score = s;

    _logFile << _id << "," << s << "," << timestamp << "\n";

    _onScoreUpdate(_score);
}

void Node::initializePatientZero()
{
    _recoverRate = 0;
    _reshareRate = 1;
    setScore(1, 0);
}

void Node::addMessage(int type, double weight) { _messages.emplace(type, weight); }

void Node::update(double engagementFactor, double timestamp)
{
    if (_state == BOT) {
        return;
    }

    if (_score != -1) {
        bool canDebunk = false;

        while (!_messages.empty()) {
            const Message& msg = _messages.front();
            double messageFactor;

            if (msg.type == -1) {
                messageFactor = 0.1;
            } else {
                messageFactor = 1;
                canDebunk = true;
            }

            double p = randomUniform(0, 1);

            if (p < engagementFactor * messageFactor * msg.weight * _vulnerability) {
                setScore(msg.type, timestamp);
                canDebunk = false;
            }

            _messages.pop();
        }

        if (canDebunk && _score == 0) {
            tryRecover(timestamp);
        }
    }
}

void Node::connect(size_t to, double weight) { _outgoingEdges.emplace_back(to, weight); }

double Node::spatialDistance(const Node& other) const
{
    return euclideanDistance(_coordinates, other._coordinates) / std::sqrt(2);
}

double Node::attributeDistance(const Node& other) const
{
    size_t numAttrs = _interestAttributes.size();
    return euclideanDistance(_coordinates, other._coordinates) / std::sqrt(numAttrs);
}

void Node::openLogFile(const std::string &filename)
{
    _logFile.open(filename);
    _logFile << "Id,Score,Timestamp\n";
}

void Node::closeLogFile()
{
    _logFile.close();
}

double Node::generateRecoverRate()
{
    const GraphSettings& settings = GraphSettings::instance();
    return randomNormal(settings.recoverRateMean, settings.recoverRateStd, 0, 1);
}

double Node::generateVulnerability()
{
    const GraphSettings& settings = GraphSettings::instance();
    return randomNormal(settings.vulnerabilityMean, settings.vulnerabilityStd, 0, 1);
}

std::vector<double> Node::generateInterestAttributes() const
{
    const GraphSettings& settings = GraphSettings::instance();

    std::vector<double> attrs(settings.numInterests);
    std::generate(attrs.begin(), attrs.end(),
        [&settings]() { return randomNormal(settings.interestsMean, settings.interestsStd, -1, 1); });

    std::vector<double> additionalAttrs = { _recoverRate, _vulnerability, static_cast<double>(_score) };
    attrs.insert(attrs.end(), additionalAttrs.begin(), additionalAttrs.end());

    return attrs;
}

double Node::generateReshareRate()
{
    const GraphSettings& settings = GraphSettings::instance();
    return randomNormal(settings.reshareRateMean, settings.reshareRateStd, 0, 1);
}

void Node::tryRecover(double timestamp)
{
    double p = randomUniform(0, 1);
    if (p < _recoverRate) {
        setScore(-1, timestamp);
    }
}

double Node::euclideanDistance(const std::vector<double>& v1, const std::vector<double>& v2)
{
    double sum = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0, std::plus<double>(),
        [](double a, double b) { double diff = a - b; return diff * diff; });
    return std::sqrt(sum);
}

std::ofstream Node::_logFile;
