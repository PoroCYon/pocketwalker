#include "set.h"
#include "../cpu.h"

InstructionSet::InstructionSet() :
    root(
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.aH()); },
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.aL()); }
    ),
    sub_0x68(
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.a()); },
        [](const CPU& cpu) { return static_cast<uint32_t>((cpu.bH() >> 3) & 1); }
    ),
    sub_0x6C(
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.a()); },
        [](const CPU& cpu) { return static_cast<uint32_t>((cpu.bH() >> 3) & 1); }
    ),
    sub_0x6D(
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.a()); },
        [](const CPU& cpu) { return static_cast<uint32_t>((cpu.bH() >> 3) & 1); }
    ),
    sub_0x79(
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.a()); },
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.bH()); }
    ),
    sub_0x0B(
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.a()); },
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.bH()); }
    ),
    sub_0x1B(
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.a()); },
        [](const CPU& cpu) { return static_cast<uint32_t>(cpu.bH()); }
    )
{
    root.Add(0x1, 0xD, {
        "CMP.W Rs, Rd",
        2,
        {1, 0, 0, 0, 0, 0},
        [](CPU& cpu)
        {
            const uint16_t* rs = cpu.reg.Reg16(cpu.bH());
            const uint16_t* rd = cpu.reg.Reg16(cpu.bL());

            cpu.reg.SubFlags(*rd, *rs);
        }
    });

    root.Add(0x4, 0x0, {
        "BRA d:8",
        2,
        {2, 0, 0, 0, 0, 0},
        [](CPU& cpu)
        {
            const int8_t disp = static_cast<int8_t>(cpu.b());
            cpu.reg.PC += disp;
        }
    });

    root.Add(0x4, 0x5, {
        "BCS d:8",
        2,
        {2, 0, 0, 0, 0, 0},
        [](CPU& cpu)
        {
            const int8_t disp = static_cast<int8_t>(cpu.b());
            if (cpu.reg.flags.C)
                cpu.reg.PC += disp;
        }
    });

    root.Add(0x5, 0x4, {
        "RTS",
        2,
        {2, 0, 1, 0, 0, 2},
        [](CPU& cpu)
        {
            cpu.reg.PC = cpu.Pop16();
        },
        true
    });

    root.Add(0x5, 0xE, {
        "JSR @aa:24",
        4,
        {2, 0, 1, 0, 0, 2},
        [](CPU& cpu)
        {
            cpu.Push16(cpu.reg.PC + 4);

            const uint32_t abs = (cpu.b() << 16) | cpu.cd();
            cpu.reg.PC = abs;
        },
        true
    });

    root.AddSubtable(0x6, 0x8, sub_0x68);
    {
        sub_0x68.Add(0x68, 0x00, {
            "MOV.B @ERs, Rd",
            2,
            {1, 0, 0, 1, 0, 0},
            [](CPU& cpu)
            {
                uint32_t* ers = cpu.reg.Reg32(cpu.bH());
                uint8_t* rd = cpu.reg.Reg8(cpu.bL());

                *rd = cpu.memory->Read8(*ers);
                cpu.reg.MovFlags(*rd);
            }
        });

        sub_0x68.Add(0x68, 0x01, {
            "MOV.B Rs, @ERd",
            2,
            {1, 0, 0, 1, 0, 0},
            [](CPU& cpu)
            {
                uint32_t* erd = cpu.reg.Reg32(cpu.bH());
                const uint8_t* rs = cpu.reg.Reg8(cpu.bL());

                cpu.memory->Write8(*erd, *rs);
                cpu.reg.MovFlags(*rs);
            }
        });
    }


    root.AddSubtable(0x6, 0xC, sub_0x6C);
    {
        sub_0x6C.Add(0x6C, 0x00, {
            "MOV.B @ERs+, Rd",
            2,
            {1, 0, 0, 1, 0, 2},
            [](CPU& cpu)
            {
                uint32_t* ers = cpu.reg.Reg32(cpu.bH());
                uint8_t* rd = cpu.reg.Reg8(cpu.bL());

                *rd = cpu.memory->Read8(*ers);
                cpu.reg.MovFlags(*rd);

                *ers += 1;
            }
        });

        sub_0x6C.Add(0x6C, 0x01, {
            "MOV.B Rs, @-ERd",
            2,
            {1, 0, 0, 1, 0, 2},
            [](CPU& cpu)
            {
                uint32_t* erd = cpu.reg.Reg32(cpu.bH());
                const uint8_t* rs = cpu.reg.Reg8(cpu.bL());

                *erd -= 1;

                cpu.memory->Write8(*erd, *rs);
                cpu.reg.MovFlags(*rs);
            }
        });
    }

    root.AddSubtable(0x6, 0xD, sub_0x6D);
    {
        sub_0x6D.Add(0x6D, 0x00, {
            "MOV.W @ERs+, Rd",
            2,
            {1, 0, 0, 0, 1, 2},
            [](CPU& cpu)
            {
                uint32_t* ers = cpu.reg.Reg32(cpu.bH());
                uint16_t* rd = cpu.reg.Reg16(cpu.bL());

                *rd = cpu.memory->Read16(*ers);
                cpu.reg.MovFlags(*rd);

                *ers += 2;
            }
        });

        sub_0x6D.Add(0x6D, 0x01, {
            "MOV.W Rs, @-ERd",
            2,
            {1, 0, 0, 0, 1, 2},
            [](CPU& cpu)
            {
                uint32_t* erd = cpu.reg.Reg32(cpu.bH());
                const uint16_t* rs = cpu.reg.Reg16(cpu.bL());

                *erd -= 2;

                cpu.memory->Write16(*erd, *rs);
                cpu.reg.MovFlags(*rs);
            }
        });
    }


    root.AddSubtable(0x7, 0x9, sub_0x79);
    {
        sub_0x79.Add(0x79, 0x00, {
            "MOV.W #xx:16, Rd",
            4,
            {1, 0, 0, 0, 2, 2},
            [](CPU& cpu)
            {
                const uint16_t imm = cpu.cd();
                uint16_t* rd = cpu.reg.Reg16(cpu.bL());

                *rd = imm;
                cpu.reg.MovFlags(imm);
            }
        });
    }

    root.AddSubtable(0x0, 0xB, sub_0x0B);
    {
        sub_0x0B.Add(0x0B, 0x00, {
            "ADDS #1, ERd",
            2,
            {1, 0, 0, 0, 0, 0},
            [](CPU& cpu)
            {
                uint32_t* erd = cpu.reg.Reg32(cpu.bL());
                *erd += 1;
            }
        });
    }

    root.AddSubtable(0x1, 0xB, sub_0x1B);
    {
        sub_0x1B.Add(0x1B, 0x09, {
            "SUBS #4, ERd",
            2,
            {1, 0, 0, 0, 0, 0},
            [](CPU& cpu)
            {
                uint32_t* erd = cpu.reg.Reg32(cpu.bL());
                *erd -= 4;
            }
        });
    }

    root.Add(0xF, {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF}, {
        "MOV.B #xx:8, Rd",
        2,
        {1, 0, 0, 0, 0, 0},
        [](CPU& cpu)
        {
            const uint8_t imm = cpu.b();
            uint8_t* rd = cpu.reg.Reg8(cpu.aL());

            *rd = imm;
            cpu.reg.MovFlags(imm);
        }
    });
}

const Instruction* InstructionSet::Decode(const CPU& cpu) const
{
    return root.Decode(cpu);
}
