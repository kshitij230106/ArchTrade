/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Instruction types and representation for simplified program simulation.
 */

#ifndef ARCHTRADE_INSTRUCTION_H
#define ARCHTRADE_INSTRUCTION_H

/**
 * Types of instructions in our simplified ISA.
 * ALU: arithmetic/logic (add, sub, etc.)
 * LOAD/STORE: memory access
 * BRANCH: conditional jumps (cause pipeline penalty)
 * IO: I/O operations (polling vs DMA)
 */
enum class InstructionType {
    ALU,
    LOAD,
    STORE,
    BRANCH,
    IO
};

/**
 * Represents a single instruction in the simulated program.
 */
struct Instruction {
    InstructionType type;
    int id;  // Unique id for hazard detection (e.g. same "address" = potential hazard)
};

#endif // ARCHTRADE_INSTRUCTION_H