#pragma once
#include <QDialog>
#include "../../widget/keybind/key_bind_button.h"

class ControlSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ControlSettingsDialog(QWidget* parent = nullptr);

private slots:
    void apply();

private:
    KeyBindButton* left_button;
    KeyBindButton* right_button;
    KeyBindButton* center_button;
    KeyBindButton* speedup_button;
    KeyBindButton* synthetic_steps_button;
};