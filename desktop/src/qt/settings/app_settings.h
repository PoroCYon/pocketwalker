#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "types/general_settings.h"

#define MAX_RECENT_ROMS 5


class AppSettings
{
public:
    static AppSettings instance;

    void load();
    void save() const;

    GeneralSettings general;

private:
    AppSettings() = default;
    static std::string settingsPath();
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AppSettings, general)
