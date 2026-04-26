#pragma once
#include <array>
#include <QCheckBox>
#include <QDialog>
#include <QPushButton>
#include "desktop/src/qt/settings/types/emulation_settings.h"

class EmulationSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmulationSettingsDialog(QWidget* parent = nullptr);

signals:
    void paletteChanged();
    void bypassPowerSaveChanged();

private slots:
    void apply();
    void reset();

private:
    void pickColor(int index);
    void updateSwatch(int index);

    std::array<QPushButton*, 4> swatches;
    std::array<EmulationSettings::Color, 4> pending_palette;

    QCheckBox* bypass_power_save_check;
};
