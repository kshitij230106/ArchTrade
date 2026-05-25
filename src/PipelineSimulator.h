/**
 * ArchTrade - Educational Computer Architecture Simulator
 * PipelineSimulator:
 * Executes workloads using configurable architecture parameters.
 */

#ifndef ARCHTRADE_PIPELINE_SIMULATOR_H
#define ARCHTRADE_PIPELINE_SIMULATOR_H

#include "ArchitectureConfig.h"
#include "CacheSimulator.h"
#include "Instruction.h"
#include "SimulationResult.h"
#include "Workload.h"

#include <vector>

class PipelineSimulator {
private:
    CacheSimulator cache;

public:

    /**
     * Run workload simulation.
     */
    SimulationResult run(
        const Workload& workload,
        const ArchitectureConfig& config
    ) {

        cache.reset();

        SimulationResult result;

        // ============================================
        // Validate configuration
        // ============================================

        if (!config.isValid()) {
            return result;
        }

        // ============================================
        // Instruction stream
        // ============================================

        const std::vector<Instruction>& instructions =
            workload.instructions;

        result.instructionsExecuted =
            (int)instructions.size();

        if (instructions.empty()) {
            return result;
        }

        // ============================================
        // Architecture-derived values
        // ============================================

        int totalCycles =
            config.pipelineStartupCycles();

        int baseCycles =
            config.baseInstructionCycles();

        int hazardPenalty =
            config.dataHazardStall();

        // ============================================
        // Tracking
        // ============================================

        const Instruction* previousInstruction = nullptr;

        // ============================================
        // Main simulation loop
        // ============================================

        for (const Instruction& inst : instructions) {

            // ----------------------------------------
            // Base execution
            // ----------------------------------------

            totalCycles += baseCycles;

            result.executionCycles += baseCycles;

            // ----------------------------------------
            // Memory instructions
            // ----------------------------------------

            if (inst.type == InstructionType::LOAD ||
                inst.type == InstructionType::STORE) {

                bool hit =
                    cache.accessMemory(
                        inst.id,
                        config
                    );

                if (hit) {

                    totalCycles +=
                        config.cacheHitCycles();
                }
                else {

                    totalCycles +=
                        config.cacheMissCycles();

                    result.memoryStallCycles +=
                        config.ramLatency;
                }
            }

            // ----------------------------------------
            // Branch instructions
            // ----------------------------------------

            if (inst.type == InstructionType::BRANCH) {

                int penalty =
                    config.branchPenalty();

                totalCycles += penalty;

                result.branchStallCycles +=
                    penalty;

                result.pipelineStalls +=
                    penalty;
            }

            // ----------------------------------------
            // Data hazard detection
            // ----------------------------------------

            if (previousInstruction != nullptr) {

                // Simplified RAW hazard
                if (inst.id ==
                    previousInstruction->id) {

                    totalCycles +=
                        hazardPenalty;

                    result.pipelineStalls +=
                        hazardPenalty;
                }
            }

            // ----------------------------------------
            // I/O instructions
            // ----------------------------------------

            if (inst.type == InstructionType::IO) {

                if (config.ioType ==
                    IOType::Polling) {

                    totalCycles +=
                        config.deviceLatency;

                    result.ioStallCycles +=
                        config.deviceLatency;

                    result.cpuIdleCycles +=
                        config.deviceLatency;
                }

                // DMA assumed asynchronous
            }

            previousInstruction = &inst;
        }

        // ============================================
        // Final statistics
        // ============================================

        result.totalCycles =
            totalCycles;

        // CPI
        result.cpi =
            (double)totalCycles /
            result.instructionsExecuted;

        // Cache statistics
        result.cacheHits =
            cache.cacheHits;

        result.cacheMisses =
            cache.cacheMisses;

        return result;
    }
};

#endif // ARCHTRADE_PIPELINE_SIMULATOR_H