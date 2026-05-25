/**
 * ArchTrade - Educational Computer Architecture Simulator
<<<<<<< HEAD
 * PipelineSimulator:
 * Executes workloads using configurable architecture parameters.
=======
 * PipelineSimulator: runs instructions and applies parameter-driven
 * pipeline / cache / IO rules.
 *
 * Every ArchitectureConfig field influences at least one metric:
 *
 *   isRISC           → baseInstructionCycles() (decode overhead)
 *   pipelineDepth    → branchPenalty(), dataHazardStall(), pipelineStartupCycles()
 *   cacheType        → cacheHitRatePercent() → hit/miss ratio
 *   cacheSizeKB      → cacheHitRatePercent()
 *   cacheLatency     → cacheHitCycles(), cacheMissCycles()
 *   ramLatency       → cacheMissCycles(), memoryStallCycles
 *   memoryBusLatency → cacheHitCycles(), cacheMissCycles()
 *   aluLatency       → baseInstructionCycles(), dataHazardStall()
 *   decodeLatency    → baseInstructionCycles() (x2 for CISC)
 *   useDMA           → cpuIdleCycles / ioStallCycles
 *   deviceLatency    → cpuIdleCycles / ioStallCycles (polling mode)
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
 */

#ifndef ARCHTRADE_PIPELINE_SIMULATOR_H
#define ARCHTRADE_PIPELINE_SIMULATOR_H

#include "ArchitectureConfig.h"
#include "CacheSimulator.h"
#include "Instruction.h"
#include "SimulationResult.h"
#include "Workload.h"
<<<<<<< HEAD

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
=======
#include <cstdlib>
#include <ctime>
#include <vector>

class PipelineSimulator {
public:
    PipelineSimulator() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    /**
     * Run the workload with the given config; fill and return result.
     */
    SimulationResult run(const Workload& workload, const ArchitectureConfig& config) {
        cache_.reset();
        SimulationResult r;
        r.instructionsExecuted = static_cast<int>(workload.instructions.size());

        const bool isPipeline = (config.pipelineDepth >= 2);

        // ── Derived parameters (all computed from config, no magic numbers) ──
        const int branchPenalty  = config.branchPenalty();     // 0 for single-cycle
        const int hazardStall    = config.dataHazardStall();   // = aluLatency
        const int baseCycles     = config.baseInstructionCycles(); // decode+execute (CISC pays extra)
        const int cacheHitCost   = config.cacheHitCycles();    // cacheLatency + memBus
        const int cacheMissCost  = config.cacheMissCycles();   // + ramLatency
        const int memBus         = config.memoryBusLatency;    // per-op bus overhead
        const int deviceLatency  = config.deviceLatency;       // polling idle per IO

        // Pipeline startup cost (fill latency at beginning of program)
        int startupCycles = config.pipelineStartupCycles();

        const std::vector<Instruction>& instr = workload.instructions;
        int prevWasLoad = -1; // id of previous LOAD instruction (-1 = none)

        int cycles        = startupCycles;
        int stalls        = 0;
        int idleCycles    = 0;
        int memStalls     = 0;
        int branchStalls  = 0;
        int execCycles    = 0;

        for (size_t i = 0; i < instr.size(); ++i) {
            const Instruction& inst = instr[i];

            // ── Branch penalty (pipelined CPUs only) ──────────────────────
            if (inst.type == InstructionType::BRANCH && isPipeline) {
                int bp = branchPenalty;
                stalls       += bp;
                branchStalls += bp;
            }

            // ── Data hazard: use-after-load ───────────────────────────────
            if (isPipeline && prevWasLoad >= 0) {
                // 30 % chance of a RAW hazard on ALU/STORE following a LOAD
                if (inst.type == InstructionType::ALU ||
                    inst.type == InstructionType::STORE) {
                    if ((std::rand() % 100) < 30) {
                        stalls += hazardStall;
                    }
                }
            }
            prevWasLoad = (inst.type == InstructionType::LOAD) ? inst.id : -1;

            // ── Memory: cache hit / miss ──────────────────────────────────
            if (inst.type == InstructionType::LOAD ||
                inst.type == InstructionType::STORE) {
                bool hit = cache_.accessMemory(inst.id, config);
                if (hit) {
                    cycles += cacheHitCost;
                } else {
                    cycles    += cacheMissCost;
                    memStalls += config.ramLatency; // miss stall
                }
                // Memory bus latency is always paid (included in hit/miss cost),
                // but we also charge it explicitly for a STORE's address phase.
                if (inst.type == InstructionType::STORE) {
                    cycles += memBus;
                }
            }

            // ── I/O: polling vs DMA ───────────────────────────────────────
            if (inst.type == InstructionType::IO) {
                if (!config.useDMA) {
                    idleCycles += deviceLatency;
                }
                // DMA: no CPU stall, but still costs base cycles to issue
            }

            // ── Base execute + decode ─────────────────────────────────────
            cycles     += baseCycles;
            execCycles += baseCycles;
        }

        r.totalCycles       = cycles + stalls + idleCycles;
        r.pipelineStalls    = stalls;
        r.cpuIdleCycles     = idleCycles;
        r.cacheHits         = cache_.cacheHits;
        r.cacheMisses       = cache_.cacheMisses;
        r.memoryStallCycles = memStalls;
        r.branchStallCycles = branchStalls;
        r.ioStallCycles     = idleCycles;
        r.executionCycles   = execCycles;
        r.cpi = (r.instructionsExecuted > 0)
            ? static_cast<double>(r.totalCycles) / r.instructionsExecuted
            : 0.0;

        return r;
    }

private:
    CacheSimulator cache_;
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
};

#endif // ARCHTRADE_PIPELINE_SIMULATOR_H