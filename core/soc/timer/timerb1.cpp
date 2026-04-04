#include "timerb1.h"

#include "core/soc/defines.h"

static std::array<uint32_t, 8> clock_rates = {
    8192,
    2048,
    256,
    64,
    16,
    4,
    PHI_CLK / PHI_W_CLK * 1024,
    PHI_CLK / PHI_W_CLK * 256
};

TimerB1::TimerB1(const std::shared_ptr<MemoryInterface>& memory)
{
    mem = memory;
}

void TimerB1::RegisterIOHandlers(const std::shared_ptr<IO>& io)
{
    io->RegisterReadHandler(TIMER_B1_ADDR_TMB1, [this]
    {
        return TMB1.VALUE;
    });

    io->RegisterWriteHandler(TIMER_B1_ADDR_TMB1, [this](const uint8_t value)
    {
        TMB1.VALUE = value;
    });

    io->RegisterReadHandler(TIMER_B1_ADDR_TCB1, [this]
    {
        return TCB1;
    });

    io->RegisterWriteHandler(TIMER_B1_ADDR_TCB1, [this](const uint8_t value)
    {
        TLB1 = value;
    });
}

void TimerB1::Cycle(uint8_t cycles)
{
    if (!TMB1.CNT)
        return;

    timer_b1_cycles += cycles;
    if (timer_b1_cycles >= clock_rates[TMB1.CKS])
    {
        timer_b1_cycles -= clock_rates[TMB1.CKS];

        if (TCB1 == 0xFF)
        {
            TCB1 = TLB1;
            // TODO add interrrupt flag
        }
        else
        {
            TCB1++;
        }
    }
}
