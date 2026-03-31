#include "h838606.h"

#include <array>
#include <cstdint>
#include <memory>
#include <print>

H838606::H838606(RomBuffer rom_buffer)
{
    auto rom = std::make_shared<ROM>(rom_buffer);
    auto ram = std::make_shared<RAM>();
    auto io = std::make_shared<IO>();

    this->memory = std::make_shared<MemoryBus>(rom, ram, io);
    this->cpu = std::make_unique<CPU>(this->memory);
}

void H838606::Run()
{
    while (true)
    {
        uint8_t cycles = this->cpu->Cycle();
    }
}
