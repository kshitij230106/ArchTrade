/**
 * ArchTrade - Educational Computer Architecture Simulator
 * Workload implementation:
 * generates instruction streams for simulation.
 */

#include "Workload.h"

#include <random>

/**
 * Create standard workloads.
 */
Workload Workload::create(WorkloadType type)
{
    std::vector<Instruction> instructions;

    std::string name;

    int id = 0;

    // Random generator
    std::mt19937 rng(std::random_device{}());

    // =====================================================
    // Fibonacci
    // =====================================================

    if (type == WorkloadType::Fibonacci) {

        name = "Fibonacci (branch heavy)";

        const int loopIterations = 80;

        for (int i = 0; i < loopIterations; ++i) {

            Instruction alu1(
                InstructionType::ALU,
                id++
            );

            alu1.destinationRegister = 1;
            alu1.sourceRegister1 = 0;

            instructions.push_back(alu1);

            Instruction branch1(
                InstructionType::BRANCH,
                id++
            );

            branch1.branchTaken =
                (i % 2 == 0);

            instructions.push_back(branch1);

            Instruction alu2(
                InstructionType::ALU,
                id++
            );

            alu2.destinationRegister = 2;
            alu2.sourceRegister1 = 1;

            instructions.push_back(alu2);

            Instruction branch2(
                InstructionType::BRANCH,
                id++
            );

            branch2.branchTaken =
                (i % 3 == 0);

            instructions.push_back(branch2);

            // Occasional I/O
            if (i % 20 == 19) {

                Instruction ioInst(
                    InstructionType::IO,
                    id++
                );

                instructions.push_back(ioInst);
            }
        }

        Instruction finalIO(
            InstructionType::IO,
            id++
        );

        instructions.push_back(finalIO);
    }

    // =====================================================
    // Array Sum
    // =====================================================

    else if (type == WorkloadType::ArraySum) {

        name = "Array Sum (good cache locality)";

        const int arraySize = 200;

        for (int i = 0; i < arraySize; ++i) {

            Instruction load(
                InstructionType::LOAD,
                id++
            );

            load.memoryAddress = i;

            instructions.push_back(load);

            Instruction alu(
                InstructionType::ALU,
                id++
            );

            alu.destinationRegister = 1;
            alu.sourceRegister1 = 0;

            instructions.push_back(alu);
        }

        Instruction store(
            InstructionType::STORE,
            id++
        );

        store.memoryAddress = arraySize;

        instructions.push_back(store);

        Instruction ioInst(
            InstructionType::IO,
            id++
        );

        instructions.push_back(ioInst);
    }

    return Workload(
        name,
        std::move(instructions)
    );
}

/**
 * Bubble Sort workload.
 */
Workload Workload::createBubbleSort()
{
    Workload workload(
        "Bubble Sort (branch + memory heavy)"
    );

    for (int i = 0; i < 120; ++i) {

        workload.addInstruction(
            InstructionType::LOAD
        );

        workload.addInstruction(
            InstructionType::LOAD
        );

        workload.addInstruction(
            InstructionType::ALU
        );

        workload.addInstruction(
            InstructionType::BRANCH
        );

        workload.addInstruction(
            InstructionType::STORE
        );

        // Periodic I/O
        if (i % 30 == 29) {

            workload.addInstruction(
                InstructionType::IO
            );
        }
    }

    return workload;
}

/**
 * Random memory workload.
 */
Workload Workload::createRandomMemory()
{
    Workload workload(
        "Random Memory Access (poor cache locality)"
    );

    for (int i = 0; i < 150; ++i) {

        workload.addInstruction(
            InstructionType::LOAD
        );

        workload.addInstruction(
            InstructionType::ALU
        );

        workload.addInstruction(
            InstructionType::LOAD
        );

        workload.addInstruction(
            InstructionType::ALU
        );

        workload.addInstruction(
            InstructionType::LOAD
        );

        // Periodic I/O
        if (i % 25 == 24) {

            workload.addInstruction(
                InstructionType::IO
            );
        }
    }

    return workload;
}

/**
 * I/O-heavy workload.
 */
Workload Workload::createIOProcessing()
{
    Workload workload(
        "I/O Processing (I/O heavy)"
    );

    for (int i = 0; i < 100; ++i) {

        workload.addInstruction(
            InstructionType::ALU
        );

        workload.addInstruction(
            InstructionType::IO
        );

        workload.addInstruction(
            InstructionType::IO
        );

        workload.addInstruction(
            InstructionType::LOAD
        );

        workload.addInstruction(
            InstructionType::IO
        );
    }

    return workload;
}