#pragma once
#include <string>
#include <vector>

struct OutcomeCategory {
    std::string label;
    int weight;
};

struct ProbabilityClass {
    std::string classLabel;
    double initialProbability;
    double incrementalBiasRate;
    int consecutiveFailures = 0;
    int deterministicLimit;
    std::vector<OutcomeCategory> categoryOutcomes;
};

std::string selectWeightedOutcome(const std::vector<OutcomeCategory>& pool);
std::string fairXORNG_Evaluate(std::vector<ProbabilityClass>& classes); 