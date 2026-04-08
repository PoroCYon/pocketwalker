#pragma once
#include <array>
#include <cstdint>
#include <memory>

#include "core/soc/h838606.h"
#include "core/utils/event_handler.h"
#include "peripherals/bma150/bma150.h"
#include "peripherals/buzzer/buzzer.h"
#include "peripherals/m95512/m95512.h"
#include "peripherals/ssd1854/ssd1854.h"

#define PW_ADDR_WATTS 0xF78E

enum class ButtonType
{
    CENTER = 1 << 0,
    LEFT   = 1 << 2,
    RIGHT  = 1 << 4
};

class PocketWalker
{
public:
    PocketWalker(RomBuffer rom_buffer);

    void Start();
    void Stop();

    void SetWatts(uint16_t value);

    void OnSamplePushed(const EventHandlerCallback<BuzzerInformation>& callback);

    void OnTransmitIR(const EventHandlerCallback<uint8_t>& callback);
    void ReceiveIR(uint8_t data);

    SSD1854DrawInfo* GetDrawInfo();

    void PressButton(ButtonType button) const;
    void ReleaseButton(ButtonType button) const;

    EepromBuffer GetEepromBuffer() const;
    void SetEepromBuffer(const EepromBuffer& buffer) const;

private:
    std::shared_ptr<H838606> soc = nullptr;

    std::shared_ptr<BMA150> bma150 = nullptr;
    std::shared_ptr<M95512> m95512 = nullptr;
    std::shared_ptr<SSD1854> ssd1854 = nullptr;
    std::shared_ptr<Buzzer> buzzer = nullptr;

    bool is_running = false;;
};