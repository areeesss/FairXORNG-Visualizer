#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <cstdint>
#include <algorithm>

// Forward declarations
class XORShift64;
class XORShift128Plus;

// Game Probability System
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

// RNG Implementations
class XORShift64 {
private:
    uint64_t state;

public:
    explicit XORShift64(uint64_t seed = 1);
    uint64_t next();
    double nextDouble();
    uint32_t nextInt(uint32_t max);
};

class XORShift128Plus {
private:
    uint64_t state[2];

public:
    XORShift128Plus(uint64_t seed1, uint64_t seed2);
    uint64_t next();
    double nextDouble();
};

// Function declarations
std::string selectWeightedOutcome(const std::vector<OutcomeCategory>& pool, XORShift64& rng);
std::string fairXORNG_Evaluate_Hybrid(std::vector<ProbabilityClass>& classes); 