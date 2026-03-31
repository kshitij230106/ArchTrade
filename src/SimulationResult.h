/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Simulation result: all metrics produced by a single run.
 */

#ifndef ARCHTRADE_SIMULATION_RESULT_H
#define ARCHTRADE_SIMULATION_RESULT_H

/**
 * Aggregates performance metrics from PipelineSimulator and CacheSimulator.
 * Used for final console output and web API response.
 */
struct SimulationResult {
    int instructionsExecuted = 0;
    int totalCycles          = 0;
    double cpi               = 0.0;
    int pipelineStalls       = 0;
    int cacheHits            = 0;
    int cacheMisses          = 0;
    int cpuIdleCycles        = 0;

    // Extended metrics
    int memoryStallCycles    = 0;  // extra cycles from cache misses
    int branchStallCycles    = 0;  // branch penalty cycles
    int ioStallCycles        = 0;  // IO idle cycles (same as cpuIdleCycles for clarity)
    int executionCycles      = 0;  // base decode+execute cycles (no stalls/idle)
};

#endif // ARCHTRADE_SIMULATION_RESULT_H