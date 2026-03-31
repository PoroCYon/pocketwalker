#pragma once
#include "table.h"

class InstructionSet
{
public:
    InstructionSet();

    const Instruction* Decode(const CPU& cpu) const;

private:
    InstructionTable root;

    InstructionTable sub_0x68;
    InstructionTable sub_0x6C;
    InstructionTable sub_0x6D;
    InstructionTable sub_0x79;
    InstructionTable sub_0x0B;
    InstructionTable sub_0x1B;
};