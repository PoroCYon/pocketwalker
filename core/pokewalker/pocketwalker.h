#pragma once
#include <array>
#include <cstdint>
#include <memory>

#include "core/soc/h838606.h"

class PocketWalker
{
public:
    PocketWalker(RomBuffer rom_buffer, std::array<uint8_t, 0xFFFF> save_buffer);

    void Start();

private:
    std::unique_ptr<H838606> soc = nullptr;
};
