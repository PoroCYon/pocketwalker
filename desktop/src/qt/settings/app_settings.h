#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "types/general_settings.h"
#include "types/ir_settings.h"

#define MAX_RECENT_ROMS 5

class AppSettings
{
public:
    static AppSettings instance;

    void load();
    void save() const;

    GeneralSettings general;
    IRSettings ir;

private:
    AppSettings() = default;
    static std::string settingsPath();
};

inline void to_json(nlohmann::json& j, const AppSettings& s)
{
    j = nlohmann::json{
            {"general", s.general},
            {"ir", s.ir}
    };
}

inline void from_json(const nlohmann::json& j, AppSettings& s)
{
    if (j.contains("general"))
        j.at("general").get_to(s.general);
    if (j.contains("ir"))
        j.at("ir").get_to(s.ir);
}