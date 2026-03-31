#pragma once
#include "core/cpu/cpu.h"
#include "memory/bus.h"
#include "memory/regions/rom.h"

class H838606
{
public:
    explicit H838606(RomBuffer rom_buffer);

    void Run();

private:
    std::shared_ptr<MemoryBus> memory = nullptr;
    std::shared_ptr<CPU> cpu = nullptr;
};
