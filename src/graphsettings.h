#pragma once

class GraphSettings {
public:
    size_t numInterests       = 5;
    double interestsMean      = 0;
    double interestsStd       = 0.4;
    double recoverRateMean    = 0.2;
    double recoverRateStd     = 0.2;
    double vulnerabilityMean  = 0.5;
    double vulnerabilityStd   = 0.2;
    double reshareRateMean    = 0.5;
    double reshareRateStd     = 0.2;
    size_t numRegular         = 200;
    size_t numInfluencers     = 3;
    size_t numBots            = 3;
    size_t numFactCheckers    = 0;
    double spatialThreshold   = 0.1;
    double attrThreshold      = 0.05;
    double simTime            = 1500;
    bool isGraphWeighted      = true;
    bool recoveredNodesDebunk = true;

    static GraphSettings& instance()
    {
        static GraphSettings settings;
        return settings;
    }

private:
    GraphSettings() = default;
    GraphSettings(const GraphSettings&) = delete;
    GraphSettings& operator=(const GraphSettings&) = delete;
};
