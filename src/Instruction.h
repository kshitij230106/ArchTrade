/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Instruction representation and instruction metadata.
 */

#ifndef ARCHTRADE_INSTRUCTION_H
#define ARCHTRADE_INSTRUCTION_H

#include <string>

/**
 * Types of instructions in the simplified ISA.
 */
enum class InstructionType {
    ALU,
    LOAD,
    STORE,
    BRANCH,
    IO,
    NOP
};

/**
 * Pipeline hazard types.
 */
enum class HazardType {
    NONE,
    DATA,
    CONTROL,
    STRUCTURAL
};

/**
 * Represents a single instruction in the simulated program.
 */
class Instruction {
public:

    // =========================================================
    // Core instruction metadata
    // =========================================================

    InstructionType type = InstructionType::ALU;

    // Unique instruction identifier
    int id = 0;

    // Program counter / instruction address
    int pc = 0;

    // Simulated memory address (for LOAD/STORE)
    int memoryAddress = -1;

    // Number of cycles required
    int executionLatency = 1;

    // =========================================================
    // Dependency / hazard tracking
    // =========================================================

    // Destination register
    int destinationRegister = -1;

    // Source registers
    int sourceRegister1 = -1;
    int sourceRegister2 = -1;

    // Hazard classification
    HazardType hazardType = HazardType::NONE;

    // =========================================================
    // Branch metadata
    // =========================================================

    // Whether branch is taken
    bool branchTaken = false;

    // Branch target PC
    int branchTarget = -1;

    // =========================================================
    // Constructors
    // =========================================================

    Instruction() = default;

    Instruction(
        InstructionType instructionType,
        int instructionId
    )
        : type(instructionType),
          id(instructionId) {}

    // =========================================================
    // Helper methods
    // =========================================================

    /**
     * Check whether instruction accesses memory.
     */
    bool isMemoryInstruction() const {

        return
            type == InstructionType::LOAD ||
            type == InstructionType::STORE;
    }

    /**
     * Check whether instruction is a branch.
     */
    bool isBranchInstruction() const {

        return type == InstructionType::BRANCH;
    }

    /**
     * Check whether instruction performs I/O.
     */
    bool isIOInstruction() const {

        return type == InstructionType::IO;
    }

    /**
     * Check whether instruction is a NOP.
     */
    bool isNOP() const {

        return type == InstructionType::NOP;
    }

    /**
     * Detect RAW dependency with another instruction.
     */
    bool dependsOn(const Instruction& other) const {

        if (other.destinationRegister < 0)
            return false;

        return
            sourceRegister1 == other.destinationRegister ||
            sourceRegister2 == other.destinationRegister;
    }

    /**
     * Convert instruction type to readable string.
     */
    std::string typeToString() const {

        switch (type) {

            case InstructionType::ALU:
                return "ALU";

            case InstructionType::LOAD:
                return "LOAD";

            case InstructionType::STORE:
                return "STORE";

            case InstructionType::BRANCH:
                return "BRANCH";

            case InstructionType::IO:
                return "IO";

            case InstructionType::NOP:
                return "NOP";

            default:
                return "UNKNOWN";
        }
    }

    /**
     * Convert hazard type to readable string.
     */
    std::string hazardToString() const {

        switch (hazardType) {

            case HazardType::NONE:
                return "NONE";

            case HazardType::DATA:
                return "DATA";

            case HazardType::CONTROL:
                return "CONTROL";

            case HazardType::STRUCTURAL:
                return "STRUCTURAL";

            default:
                return "UNKNOWN";
        }
    }
};

#endif // ARCHTRADE_INSTRUCTION_H