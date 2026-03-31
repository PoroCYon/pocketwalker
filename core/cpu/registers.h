#pragma once
#include <array>
#include <cstdint>

#define REG_SP_INDEX 7

#define NEGATIVE_MASK(bits) (1 << (bits - 1))

typedef union flags
{
    uint8_t CCR = 0b10000000; // interrupt flag is set by default
    struct
    {
        bool C : 1;
        bool V : 1;
        bool Z : 1;
        bool N : 1;
        bool U : 1;
        bool H : 1;
        bool UI : 1;
        bool I : 1;
    };
} flags_t;

class Registers
{
public:
    Registers();

    uint8_t* Reg8(uint8_t select);
    uint16_t* Reg16(uint8_t select);
    uint32_t* Reg32(uint8_t select);

    template <typename T>
    void MovFlags(T value, size_t bits = sizeof(T) * 8)
    {
        const uint32_t negativeMask = NEGATIVE_MASK(bits);

        flags.N = value & negativeMask;
        flags.Z = value == 0;
        flags.V = false;
    }

    template <typename T>
    void SubFlags(T rdValue, T rsValue, size_t bits = sizeof(T) * 8)
    {
        const uint32_t negativeMask = NEGATIVE_MASK(bits);
        const uint32_t result = rdValue - rsValue;

        flags.Z = result == 0;
        flags.N = result & negativeMask;
        flags.V = ((rdValue ^ rsValue) & (rdValue ^ result) & negativeMask) != 0;
        flags.C = rsValue > rdValue;
        flags.H = ((rdValue ^ rsValue ^ result) >> (bits / 2 - 1) & 1) != 0;
    }

    uint16_t PC;
    uint32_t* SP;
    flags_t flags;

private:
    std::array<uint32_t, 8> registers = {};
};
