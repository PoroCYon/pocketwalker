#pragma once
#include <array>
#include <cstdint>
#include <nlohmann/json.hpp>

struct EmulationSettings
{
    struct Color
    {
        uint8_t r, g, b;
    };

    std::array<Color, 4> palette = {
        {
            {0xCC, 0xCC, 0xCC},
            {0x99, 0x99, 0x99},
            {0x66, 0x66, 0x66},
            {0x33, 0x33, 0x33}
        }
    };
};

inline void to_json(nlohmann::json& j, const EmulationSettings::Color& c)
{
    j = nlohmann::json{{"r", c.r}, {"g", c.g}, {"b", c.b}};
}

inline void from_json(const nlohmann::json& j, EmulationSettings::Color& c)
{
    c.r = j.value("r", static_cast<uint8_t>(0));
    c.g = j.value("g", static_cast<uint8_t>(0));
    c.b = j.value("b", static_cast<uint8_t>(0));
}

inline void to_json(nlohmann::json& j, const EmulationSettings& s)
{
    j = nlohmann::json{{"palette", s.palette}};
}

inline void from_json(const nlohmann::json& j, EmulationSettings& s)
{
    if (j.contains("palette") && j["palette"].is_array() && j["palette"].size() == 4)
        s.palette = j["palette"].get<std::array<EmulationSettings::Color, 4>>();
}
