/**
 * ArchTrade - Educational Computer Architecture Simulator
<<<<<<< HEAD
 * Workload:
 * Named sequence of instructions for program simulation.
=======
 * Workload: named sequence of instructions for different program profiles.
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
 */

#ifndef ARCHTRADE_WORKLOAD_H
#define ARCHTRADE_WORKLOAD_H

#include "Instruction.h"
<<<<<<< HEAD

=======
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
#include <string>
#include <vector>

/**
<<<<<<< HEAD
 * Supported workload types.
 */
enum class WorkloadType {

    // Branch-heavy workload
    Fibonacci,

    // Sequential memory workload
=======
 * Workload types for educational comparison:
 * - Fibonacci: branch-heavy (many BRANCH instructions)
 * - ArraySum: good cache locality (sequential LOAD/STORE, many ALU)
 */
enum class WorkloadType {
    Fibonacci,
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
    ArraySum
};

/**
<<<<<<< HEAD
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

=======
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
>>>>>>> fe6656cc5573ce4a019edd25d5b2d55b50053d6c
    int nextId_ = 0;
};

#endif // ARCHTRADE_WORKLOAD_H