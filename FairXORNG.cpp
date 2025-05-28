#include "FairXORNG.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdint>
#include <algorithm>

struct OutcomeCategory {
    std::string label;
    int weight; // Weight for intra-class selection 
};

struct ProbabilityClass {
    std::string classLabel;
    double initialProbability;      // Base drop likelihood (%)
    double incrementalBiasRate;     // Bias gain per failure
    int consecutiveFailures = 0;    // Number of failed attempts
    int deterministicLimit;         // Hard threshold for forced success
    std::vector<OutcomeCategory> categoryOutcomes;
};

// ======================
// RNG Implementations
// ======================

XORShift64::XORShift64(uint64_t seed) : state(seed) {
    if (seed == 0) state = 1;
}

uint64_t XORShift64::next() {
    state ^= state << 21;
    state ^= state >> 35;
    state ^= state << 4;
    return state;
}

double XORShift64::nextDouble() {
    return next() / static_cast<double>(UINT64_MAX);
}

uint32_t XORShift64::nextInt(uint32_t max) {
    return static_cast<uint32_t>(next() % max) + 1;
}

XORShift128Plus::XORShift128Plus(uint64_t seed1, uint64_t seed2) {
    state[0] = seed1 == 0 ? 1 : seed1;
    state[1] = seed2 == 0 ? 2 : seed2;
}

uint64_t XORShift128Plus::next() {
    uint64_t x = state[0];
    uint64_t y = state[1];
    state[0] = y;
    x ^= x << 23;
    state[1] = x ^ y ^ (x >> 17) ^ (y >> 26);
    return state[1] + y;
}

double XORShift128Plus::nextDouble() {
    return next() / static_cast<double>(UINT64_MAX);
}

// ======================
// Game Probability System
// ======================

std::string selectWeightedOutcome(const std::vector<OutcomeCategory>& pool, XORShift64& rng) {
    if (pool.empty()) return "No Selection";

    int totalWeight = 0;
    for (const auto& item : pool) {
        totalWeight += item.weight;
    }

    if (totalWeight == 0) return "No Selection";

    uint32_t roll = rng.nextInt(totalWeight);
    int cumulative = 0;

    for (const auto& item : pool) {
        cumulative += item.weight;
        if (roll <= cumulative) {
            return item.label;
        }
    }

    return pool.back().label;
}

std::string fairXORNG_Evaluate_Hybrid(std::vector<ProbabilityClass>& classes) {
    // Main high-quality RNG for probability checks
    static XORShift128Plus mainRng(
        std::chrono::high_resolution_clock::now().time_since_epoch().count(),
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    
    // Lightweight RNG for outcome selection (seeded from main RNG)
    static XORShift64 selectionRng(mainRng.next());

    for (auto& pClass : classes) {
        // Deterministic override check
        if (pClass.consecutiveFailures >= pClass.deterministicLimit) {
            pClass.consecutiveFailures = 0;
            return "[Deterministic] " + selectWeightedOutcome(pClass.categoryOutcomes, selectionRng);
        }

        // Adaptive probability calculation
        double adjustedProbability = pClass.initialProbability + 
                                   (pClass.consecutiveFailures * pClass.incrementalBiasRate);
        adjustedProbability = std::min(adjustedProbability, 100.0);

        // High-quality probability roll
        double roll = mainRng.nextDouble() * 100.0;

        if (roll < adjustedProbability) {
            // Success: reset counter and select outcome
            pClass.consecutiveFailures = 0;
            std::string result = selectWeightedOutcome(pClass.categoryOutcomes, selectionRng);
            
            // Reseed selection RNG for next time
            selectionRng = XORShift64(mainRng.next());
            return result;
        } else {
            // Failure: increment counter
            pClass.consecutiveFailures++;
        }
    }

    return "No Selection";
}