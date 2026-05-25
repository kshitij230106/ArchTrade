/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Workload:
 * Named sequence of instructions for program simulation.
 */

#ifndef ARCHTRADE_WORKLOAD_H
#define ARCHTRADE_WORKLOAD_H

#include "Instruction.h"

#include <string>
#include <vector>

/**
 * Supported workload types.
 */
enum class WorkloadType {

    // Branch-heavy workload
    Fibonacci,

    // Sequential memory workload
    ArraySum
};

/**
 * Workload:
 * collection of instructions representing a program.
 */
class Workload {
public:

    // =====================================================
    // Public fields
    // =====================================================

    std::string name;

    std::vector<Instruction> instructions;

    // =====================================================
    // Constructors
    // =====================================================

    Workload() = default;

    explicit Workload(
        const std::string& workloadName
    )
        : name(workloadName),
          nextId_(0) {}

    Workload(
        const std::string& workloadName,
        std::vector<Instruction> instructionList
    )
        : name(workloadName),
          instructions(std::move(instructionList)),
          nextId_(0) {}

    // =====================================================
    // Instruction helpers
    // =====================================================

    /**
     * Add instruction automatically assigning ID.
     */
    void addInstruction(
        InstructionType type
    ) {

        Instruction inst(
            type,
            nextId_++
        );

        instructions.push_back(inst);
    }

    /**
     * Add fully configured instruction.
     */
    void addInstruction(
        const Instruction& instruction
    ) {

        instructions.push_back(instruction);

        ++nextId_;
    }

    // =====================================================
    // Factory methods
    // =====================================================

    /**
     * Create standard workload.
     */
    static Workload create(
        WorkloadType type
    );

    /**
     * Branch + memory heavy workload.
     */
    static Workload createBubbleSort();

    /**
     * Poor cache locality workload.
     */
    static Workload createRandomMemory();

    /**
     * I/O heavy workload.
     */
    static Workload createIOProcessing();

private:

    // =====================================================
    // Internal instruction ID counter
    // =====================================================

    int nextId_ = 0;
};

#endif // ARCHTRADE_WORKLOAD_H