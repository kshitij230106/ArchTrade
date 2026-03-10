/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Workload: named sequence of instructions for different program profiles.
 */

#ifndef ARCHTRADE_WORKLOAD_H
#define ARCHTRADE_WORKLOAD_H

#include "Instruction.h"
#include <string>
#include <vector>

/**
 * Workload types for educational comparison:
 * - Fibonacci: branch-heavy (many BRANCH instructions)
 * - ArraySum: good cache locality (sequential LOAD/STORE, many ALU)
 */
enum class WorkloadType {
    Fibonacci,
    ArraySum
};

/**
 * A workload has a name and a list of instructions.
 * Static factory methods generate instruction sequences per workload type.
 */
class Workload {
public:
    std::string name;
    std::vector<Instruction> instructions;

    Workload() = default;
    explicit Workload(const std::string& n) : name(n), nextId_(0) {}
    Workload(const std::string& n, std::vector<Instruction> instr)
        : name(n), instructions(std::move(instr)), nextId_(0) {}

    /** Append one instruction (assigns next id automatically). */
    void addInstruction(InstructionType type) {
        instructions.push_back({ type, nextId_++ });
    }

    /**
     * Build a workload of the given type with a reasonable instruction count.
     */
    static Workload create(WorkloadType type);

    /** New workloads (branch+memory, poor locality, I/O heavy). */
    static Workload createBubbleSort();
    static Workload createRandomMemory();
    static Workload createIOProcessing();

private:
    int nextId_ = 0;
};

#endif // ARCHTRADE_WORKLOAD_H
