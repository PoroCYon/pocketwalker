#include "pocketwalker.h"

PocketWalker::PocketWalker(RomBuffer rom_buffer, std::array<uint8_t, 0xFFFF> save_buffer)
{
    this->soc = std::make_unique<H838606>(rom_buffer);
}

void PocketWalker::Start()
{
    this->soc->Run();
}
