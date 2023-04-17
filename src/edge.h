#pragma once

struct Edge {
    size_t src;
    size_t dest;
    double weight;

    Edge(size_t s, size_t d, double w)
        : src(s)
        , dest(d)
        , weight(w)
    {
    }
};
