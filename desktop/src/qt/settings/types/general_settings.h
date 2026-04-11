#pragma once


struct GeneralSettings
{
    enum class AppTheme
    {
        System,
        Light,
        Dark
    };

    AppTheme theme = AppTheme::System;
    bool boot_on_launch = false;
    std::string default_rom;
    std::vector<std::string> recent_roms;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GeneralSettings, theme, boot_on_launch, default_rom, recent_roms)
