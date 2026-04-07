#include "buzzer.h"

#include "core/soc/defines.h"

uint16_t clock_rate = PHI_CLK / PHI_W_CLK;

Buzzer::Buzzer(const std::shared_ptr<TimerW>& timer_w)
{
    this->timer_w = timer_w;
}

void Buzzer::Cycle(uint8_t cycles)
{
    buzzer_cycles +=  cycles;
    while (buzzer_cycles >= clock_rate)
    {
        buzzer_cycles -= clock_rate;

        float frequency = timer_w->TMRW.CTS && *timer_w->GRA > 0
                         ? PHI_W_CLK / *timer_w->GRA
                         : 0.0f;

        bool is_full_volume = *timer_w->GRB == *timer_w->GRC;

        OnSamplePushed({frequency, is_full_volume});
    }
}
